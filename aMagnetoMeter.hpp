#ifndef AMAGNETOMETER_HPP
#define AMAGNETOMETER_HPP

#include "hwlib.hpp"
#include <array>
#include <math.h>
class aMagnetoMeter{
private:
    hwlib::i2c_bus & bus;
    uint_fast8_t address;
    
    uint_fast8_t xLSB = 0x00;
    uint_fast8_t xMSB = 0x01;
    uint_fast8_t yLSB = 0x02;
    uint_fast8_t yMSB = 0x03;
    uint_fast8_t zLSB = 0x04;
    uint_fast8_t zMSB = 0x05;
    uint_fast8_t status = 0x06;
    uint_fast8_t tLSB = 0x07;
    uint_fast8_t tMSB = 0x08;
    uint_fast8_t config = 0x09;
    uint_fast8_t config2 = 0x0a;
    uint_fast8_t reset = 0x0b;
    
  
    
    /*config data*/
    uint_fast8_t configOS512 = 0x00;
    uint_fast8_t configOS256 = 0x40;
    uint_fast8_t configOS128 = 0x80;
    uint_fast8_t configOS64 = 0xc0;
    
    uint_fast8_t config2gauss = 0x00;
    uint_fast8_t config8gauss = 0x10;
    
    uint_fast8_t config10hz = 0x00;
    uint_fast8_t config50hz = 0x04;
    uint_fast8_t config100hz = 0x08;
    uint_fast8_t config200hz = 0x0c;
    
    uint_fast8_t configStandby = 0x00;
    uint_fast8_t configContinue = 0x01;
    
    uint_fast8_t oversampling = configOS512;
    uint_fast8_t range = config8gauss;
    uint_fast8_t rate = config200hz;
    uint_fast8_t mode = configContinue;
    
    int16_t xMin = 0;
    int16_t xMax = 0;
    int16_t yMin = 0;
    int16_t yMax = 0;
    int16_t zMin = 0;
    int16_t zMax = 0;
    float Pi = 3.14159265358979323846264338327950288;
    
    void writeReg(uint_fast8_t & reg, uint_fast8_t d);
    void resetChip();
    std::array<int16_t, 3> readRaws();
      uint_fast8_t dirRegs[6] = {
        xLSB,
        xMSB,
        yLSB,
        yMSB,
        zLSB,
        zMSB
        };
public:
    aMagnetoMeter(hwlib::i2c_bus & bus, const uint_fast8_t & address = 0x0d);
    uint8_t readReg(uint_fast8_t & reg, size_t);
    void init();
    void reconfig();
    float returnX();
    std::array<bool, 3> ready();
    std::array<int16_t, 3> readDirs();
    std::array<int, 3> returnVals();
    float azimuth(int16_t y, int16_t x);
    float heading(int16_t y, int16_t x);
    int16_t getXmin();
    int16_t getXmax();
    int16_t getYmin();
    int16_t getYmax();
};

#endif // AMAGNETOMETER_HPP
