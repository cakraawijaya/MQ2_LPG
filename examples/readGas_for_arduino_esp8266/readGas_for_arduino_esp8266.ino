// Library: MQ2_LPG
// By: Devan Cakra M.W

#include "MQ2_LPG.h" // library declaration
#define MQ2PIN A0 // mq2 pin declaration

MQ2Sensor mq2(MQ2PIN); // create a new object with the name mq2 to hold the MQ2Sensor class

void setup() {
  Serial.begin(9600); // default baudrate for the Arduino and ESP8266 development boards
  mq2.begin(); // initiate mq2 sensor
}

void loop() {  
  mq2.readGas(); // reading mq2 sensor data
  mq2.viewGasData(); // print to serial monitor: gas value & status
  delay(3000); // delay for 3 seconds
}
