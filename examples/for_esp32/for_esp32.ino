// Library: MQ2_LPG
// By: Devan Cakra M.W

#include "MQ2_LPG.h" // library declaration
#define MQ2PIN 35 // mq2 pin declaration

MQ2Sensor mq2(MQ2PIN); // create a new object with the name mq2 to hold the MQ2Sensor class

#define RL 100 // 100K ohm
#define Ro 5.46
#define Volt 5.0
#define ADC 4095.0 // maximum adc resolution on ESP32 development boards
#define x 497.4177875376839
#define x1 199.150007852152
#define x2 797.3322752256328
#define y 1.0876679972710004
#define y1 1.664988323698715
#define y2 0.8990240080541785

void setup() {
  Serial.begin(115200); // default baudrate for the ESP32 development boards
  mq2.begin(); // initiate mq2 sensor

  // set calibration
  mq2.setCalibration(RL, Ro, Volt, ADC, x, x1, x2, y, y1, y2); // comment if you have calibrated
}

void loop() {  
  mq2.readGas(); // reading mq2 sensor data
  mq2.viewGasData(); // print to serial monitor: gas value & status
  delay(3000); // delay for 3 seconds
}
