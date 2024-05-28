#ifndef ANTENNA_H
#define ANTENNA_H

#include <Arduino.h>
#include <astronode.h>
#include <string.h>

struct AntennaManager {
  ASTRONODE *astronode;
  uint32_t counter;
  bool debug;
};

void AntennaManager_init(struct AntennaManager *am);
void AntennaManager_send_payload(struct AntennaManager *am, const char *payload);
void AntennaManager_poll_events(struct AntennaManager *am);

#endif