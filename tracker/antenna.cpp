#include "antenna.h"

#define ASTRONODE_SERIAL_BAUDRATE 9600
#define ASTRONODE_SERIAL Serial1

#define ASTRONODE_WLAN_SSID ""
#define ASTRONODE_WLAN_KEY ""
#define ASTRONODE_AUTH_TOKEN ""

#define ASTRONODE_GEO_LAT 0.0
#define ASTRONODE_GEO_LON 0.0

#define ASTRONODE_WITH_PLD_ACK true
#define ASTRONODE_WITH_GEO_LOC true
#define ASTRONODE_WITH_EPHEMERIS true
#define ASTRONODE_WITH_DEEP_SLEEP_EN false
#define ASTRONODE_WITH_MSG_ACK_PIN_EN false
#define ASTRONODE_WITH_MSG_RESET_PIN_EN false
#define ASTRONODE_WITH_CMD_EVENT_PIN_EN true
#define ASTRONODE_WITH_TX_PEND_EVENT_PIN_EN false

uint8_t queue_buffer[160];

void AntennaManager_init(struct AntennaManager *am) {
  if (!am->debug && am->astronode) {
    ASTRONODE_SERIAL.begin(ASTRONODE_SERIAL_BAUDRATE);
    am->astronode->begin(ASTRONODE_SERIAL);
    am->astronode->configuration_write(ASTRONODE_WITH_PLD_ACK,
                                  ASTRONODE_WITH_GEO_LOC,
                                  ASTRONODE_WITH_EPHEMERIS,
                                  ASTRONODE_WITH_DEEP_SLEEP_EN,
                                  ASTRONODE_WITH_MSG_ACK_PIN_EN,
                                  ASTRONODE_WITH_MSG_RESET_PIN_EN,
                                  ASTRONODE_WITH_CMD_EVENT_PIN_EN,
                                  ASTRONODE_WITH_TX_PEND_EVENT_PIN_EN);

    am->astronode->geolocation_write((int32_t)(ASTRONODE_GEO_LAT * 10000000), (int32_t)(ASTRONODE_GEO_LON * 10000000));
    am->astronode->configuration_save();
    Serial.println("[+] Saved configuration");

    am->astronode->configuration_read();
    if (am->astronode->config.product_id == TYPE_WIFI_DEVKIT) {
      am->astronode->wifi_configuration_write(ASTRONODE_WLAN_SSID, ASTRONODE_WLAN_KEY, ASTRONODE_AUTH_TOKEN);
    }
    String sn;
    am->astronode->serial_number_read(&sn);
    String guid;
    am->astronode->guid_read(&guid);
    Serial.println("[*] Serial number: ");
    Serial.println(sn);
    am->astronode->clear_free_payloads();
  }

  Serial.println("[+] Configured AntennaManager");
}

void AntennaManager_send_payload(struct AntennaManager *am, const char *payload) {
  if (am->debug) {
    if (strlen(payload) < 160) {
      Serial.println("[+] Enqueued message");
      strncpy((char*)&queue_buffer, payload, 160);
      am->counter++;
    } else {
      Serial.println("[-] Failed to enqueue message");
    }

  } else {
    if (am->astronode->enqueue_payload((uint8_t *)payload, strlen(payload), am->counter) == ANS_STATUS_SUCCESS) {
      Serial.println("[+] Enqueued message");
        am->counter++;
    }
  }
}

void AntennaManager_poll_events(struct AntennaManager *am) {
  if (am->debug) {
    if (strlen((char*)queue_buffer) > 0) {
      Serial.print("[+] DEBUG: Received message: ");
      Serial.println((char*)queue_buffer);
      memset(&queue_buffer, 0x0, 160); // clear buffer
    }
  } else {
    uint8_t event_type;
    am->astronode->event_read(&event_type);

    if (event_type == EVENT_MSG_ACK)
    {
      uint16_t counter_read = 0;
      if (am->astronode->read_satellite_ack(&counter_read) == ANS_STATUS_SUCCESS)
      {
        am->astronode->clear_satellite_ack();
      }
    }
    else if (event_type == EVENT_RESET)
    {
      am->astronode->clear_reset_event();
    }
  }
}