// Library: MQ2_LPG
// By: Devan Cakra M.W

#ifndef MQ2_LPG_H
#define MQ2_LPG_H

#include "Arduino.h"
#include <EEPROM.h>

class MQ2Sensor {
  public:
    MQ2Sensor(uint8_t pin);
    void begin();
    void setCalibration(float RL, float Ro, float Volt, float ADC, double x, double x1, double x2, double y, double y1, double y2);
    void saveCalibrationData();
    void loadCalibrationData();
    float readGas();
    void viewCalibrationData();
    void viewGasData();

  private:
    uint8_t _pin;
    float _RL, _Ro, _Volt, _ADC;
    double _x, _x1, _x2, _y, _y1, _y2;
    float _VRL, _Rs, _DataRo, _DataRatio, _m, _b, _ppm;    
    void VRL();
    void roCheck();
    void ratioCheck();
    void mCurve();
    void bCurve();
};

#endif