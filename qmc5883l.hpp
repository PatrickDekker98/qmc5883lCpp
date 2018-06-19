#ifndef QMC5883L_HPP
#define QMC5883L_HPP

#include "hwlib.hpp"
#include <array>
#include <math.h>
class qmc5883l
{
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
    uint_fast8_t reset1 = 0x0b;
    
  
    
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

    void writeReg(uint8_t reg, uint8_t val);
public:
    qmc5883l(hwlib::i2c_bus & bus, const uint_fast8_t & address = 0x0d);
//    int16_t x = 0;
//    int16_t y = 0;
//    int16_t z = 0;
    void init();
    void setMode(uint16_t mode,uint16_t odr,uint16_t rng,uint16_t osr);
    void readV(int16_t* x,int16_t* y,int16_t* z);
    int8_t readI(uint8_t reg);

};

#endif // QMC5883L_HPP
