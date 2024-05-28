#include <Arduino_LSM6DSOX.h>
#include <MadgwickAHRS.h>


float pitch = 0.0;
float roll = 0.0;
float yaw = 0.0;
// Quaternion variables
#define LED_PIN_1 12
#define LED_PIN_2 11
#define LED_PIN_3 10
#define LED_PIN_4 9

Madgwick filter;

unsigned long microsPerReading, microsPrevious;

int samplingRate = 100; 

float accel[3], gyro[3];
float ledState[4];

void setup() {


  Serial.begin(9600);


  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);

  while(!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  microsPerReading = 1000000 / samplingRate;
  microsPrevious = micros();
  filter.begin(samplingRate);

}

void loop() {
  unsigned long microsNow;
  microsNow = micros();

 if (microsNow - microsPrevious >= microsPerReading) {
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(accel[0], accel[1], accel[2]);
    }

    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(gyro[0], gyro[1], gyro[2]);
    }

    filter.updateIMU(gyro[0],gyro[1],gyro[2],accel[0],accel[1],accel[2]);
    
    roll = filter.getRoll();

    if(roll > 0 && roll <= 90){
      ledState[0] = 1;
      ledState[1] = 0;
      ledState[2] = 0;
      ledState[3] = 0;
    }
    if(roll > 90 && roll <= 180){
      ledState[0] = 0;
      ledState[1] = 1;
      ledState[2] = 0;
      ledState[3] = 0;
    }
    if(roll > -90 && roll <= 0){
      ledState[0] = 0;
      ledState[1] = 0;
      ledState[2] = 1;
      ledState[3] = 0;
    }
       if(roll > -180 && roll <= -90){
      ledState[0] = 0;
      ledState[1] = 0;
      ledState[2] = 0;
      ledState[3] = 1;
    }

     digitalWrite(LED_PIN_1, ledState[0]);
    digitalWrite(LED_PIN_2, ledState[1]);
    digitalWrite(LED_PIN_3, ledState[2]);
    digitalWrite(LED_PIN_4,ledState[3]);

    Serial.print("Roll : ");
    Serial.println(roll);
    

    microsPrevious = microsPrevious + microsPerReading;
  }

}