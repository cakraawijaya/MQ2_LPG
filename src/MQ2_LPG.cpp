// Library: MQ2_LPG
// By: Devan Cakra M.W

#include "MQ2_LPG.h" // library declaration

// definition of EEPROM size and starting address for storing calibration data
#define EEPROM_SIZE 64
#define EEPROM_CALIBRATION_START_ADDR 0

// constructor of MQ2Sensor class
MQ2Sensor::MQ2Sensor(uint8_t pin) {
  _pin = pin; // sensor input pin
}

// initialize sensor and EEPROM
void MQ2Sensor::begin() {
  pinMode(_pin, INPUT); // set the sensor pin as input
  EEPROM.begin(EEPROM_SIZE); // starting EEPROM with specified size
}

// save calibration data to EEPROM
void MQ2Sensor::setCalibration(float RL, float Ro, float Volt, float ADC, double x, double x1, double x2, double y, double y1, double y2) {
  EEPROM.put(EEPROM_CALIBRATION_START_ADDR, RL); // save RL
  EEPROM.put(sizeof(RL), Ro); // save Ro
  EEPROM.put(sizeof(RL) + sizeof(Ro), Volt); // save voltage
  EEPROM.put(sizeof(RL) + sizeof(Ro) + sizeof(Volt), ADC); // save ADC
  EEPROM.put(sizeof(RL) + sizeof(Ro) + sizeof(Volt) + sizeof(ADC), x); // save x
  EEPROM.put(sizeof(RL) + sizeof(Ro) + sizeof(Volt) + sizeof(ADC) + sizeof(x), x1); // save x1
  EEPROM.put(sizeof(RL) + sizeof(Ro) + sizeof(Volt) + sizeof(ADC) + sizeof(x) + sizeof(x1), x2); // save x2
  EEPROM.put(sizeof(RL) + sizeof(Ro) + sizeof(Volt) + sizeof(ADC) + sizeof(x) + sizeof(x1) + sizeof(x2), y); // save y
  EEPROM.put(sizeof(RL) + sizeof(Ro) + sizeof(Volt) + sizeof(ADC) + sizeof(x) + sizeof(x1) + sizeof(x2) + sizeof(y), y1); // save y1
  EEPROM.put(sizeof(RL) + sizeof(Ro) + sizeof(Volt) + sizeof(ADC) + sizeof(x) + sizeof(x1) + sizeof(x2) + sizeof(y) + sizeof(y1), y2); // save y2
  EEPROM.commit(); // commit data to EEPROM
}

// load calibration data from EEPROM
void MQ2Sensor::loadCalibrationData() {
  EEPROM.get(EEPROM_CALIBRATION_START_ADDR, _RL); // load RL
  EEPROM.get(sizeof(_RL), _Ro); // load Ro
  EEPROM.get(sizeof(_RL) + sizeof(_Ro), _Volt); // load voltage
  EEPROM.get(sizeof(_RL) + sizeof(_Ro) + sizeof(_Volt), _ADC); // load ADC
  EEPROM.get(sizeof(_RL) + sizeof(_Ro) + sizeof(_Volt) + sizeof(_ADC), _x); // load x
  EEPROM.get(sizeof(_RL) + sizeof(_Ro) + sizeof(_Volt) + sizeof(_ADC) + sizeof(_x), _x1); // load x1
  EEPROM.get(sizeof(_RL) + sizeof(_Ro) + sizeof(_Volt) + sizeof(_ADC) + sizeof(_x) + sizeof(_x1), _x2); // load x2
  EEPROM.get(sizeof(_RL) + sizeof(_Ro) + sizeof(_Volt) + sizeof(_ADC) + sizeof(_x) + sizeof(_x1) + sizeof(_x2), _y); // load y
  EEPROM.get(sizeof(_RL) + sizeof(_Ro) + sizeof(_Volt) + sizeof(_ADC) + sizeof(_x) + sizeof(_x1) + sizeof(_x2) + sizeof(_y), _y1); // load y1
  EEPROM.get(sizeof(_RL) + sizeof(_Ro) + sizeof(_Volt) + sizeof(_ADC) + sizeof(_x) + sizeof(_x1) + sizeof(_x2) + sizeof(_y) + sizeof(_y1), _y2); // load y2

  VRL();        // calculate output voltage
  roCheck();    // calculate Ro value
  ratioCheck(); // calculate the Rs/Ro ratio
  mCurve();     // calculate the slope of the curve
  bCurve();     // calculate the intercept of the curve
}

// calculating the output voltage VRL
float MQ2Sensor::VRL() {
  _VRL = analogRead(_pin) * (_Volt / (_ADC)); // output voltage from sensor
  return _VRL;
}

// calculating of Ro based on VRL and RL
float MQ2Sensor::roCheck() {
  _Rs = (((_Volt / (_VRL)) - 1) * (_RL)); // resistance of sensor: -1K ohm
  _DataRo = _Rs / 9.6; // Ro reference value
  return _DataRo;
}

// calculating the ratio of Rs/Ro
float MQ2Sensor::ratioCheck() {
  _Rs = ((_Volt * (_RL)) / (_VRL)) - (_RL); // resistance of sensor
  _DataRatio = _Rs / _Ro; // ratio resistance of sensor / Ro
  return _DataRatio;
}

// calculating the slope of a logarithmic curve
float MQ2Sensor::mCurve() {
  return _m = (log(_y2) - log(_y1)) / (log(_x2) - log(_x1)); // slope of the curve
}

// calculating the intercept of a logarithmic curve
float MQ2Sensor::bCurve() {
  return _b = log(_y) - (_m) * log(_x); // intercept of the curve
}

// read gas concentration in ppm
float MQ2Sensor::readGas() {
  loadCalibrationData(); // load calibration data
  _ppm = (pow(10, ((log10(_DataRatio) - (_b))) / (_m))); // calculate gas concentration
  
  // constrain ppm values within a certain range
  if (_ppm < 200) { _ppm = 200; }
  else if (_ppm > 10000) { _ppm = 10000; }
  
  return _ppm; // return the ppm value
}

// display calibration data to the serial monitor
void MQ2Sensor::viewCalibrationData() {
  loadCalibrationData(); // load calibration data
  Serial.println("\nRo Value: " + String(_DataRo)); // Ro Value
  Serial.println("Rs/Ro Value: " + String(_DataRatio)); // Rs/Ro Ratio
  Serial.println("m Value: " + String(_m)); // slope of the curve
  Serial.println("b Value: " + String(_b)); // intercept of the curve
}

// display gas concentration data to the serial monitor
void MQ2Sensor::viewGasData() {
  Serial.print("\nGas Concentration: "); Serial.print(_ppm); Serial.println("ppm"); // gas concentration
  Serial.println(_ppm > 310 ? "Status: Danger" : "Status: Safe"); // status based on thresholds
}
