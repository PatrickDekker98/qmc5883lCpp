#ifndef QMC5883L_HPP
#define QMC5883L_HPP

#include "hwlib.hpp"
#include <array>
#include <math.h>

class magnetometer {
public:
    virtual void init()=0;
    virtual void setMode(uint16_t mode,uint16_t odr,uint16_t rng,uint16_t osr)=0;
    void readV(int16_t* x,int16_t* y,int16_t* z)=0;
    int8_t readI(uint8_t reg)=0;

}

class qmc5883l{
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

    int16_t xMin ;
    int16_t xMax ;
    int16_t yMin ;
    int16_t yMax ;
    int16_t zMin ;
    int16_t zMax ;

    void writeReg(uint8_t reg, uint8_t val);
    void updateVals(int16_t x, int16_t y,int16_t z);
public:
    qmc5883l(hwlib::i2c_bus & bus, const uint_fast8_t & address = 0x0d);
//    int16_t x = 0;
//    int16_t y = 0;
//    int16_t z = 0;
    float Pi = 3.14159265358979323846264338327950288;
    void init();
    void setDeclination();
    void setMode(uint16_t mode,uint16_t odr,uint16_t rng,uint16_t osr);
    void readV(int16_t* x,int16_t* y,int16_t* z);
    void calibratedVals(int16_t* x, int16_t* y,int16_t* z);
    int8_t readI(uint8_t reg);
    float azimuth(int16_t y, int16_t x);
    float heading(int16_t* x,int16_t* y,int16_t* z);
    int16_t getXmin();
    int16_t getXmax();
    int16_t getYmin();
    int16_t getYmax();

};

#endif // QMC5883L_HPP
