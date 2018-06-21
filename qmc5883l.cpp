#include "qmc5883l.hpp"
#include <array>

qmc5883l::qmc5883l(hwlib::i2c_bus & bus, const uint_fast8_t & address):
    bus(bus),
    address(address)
{}

void qmc5883l::writeReg(uint8_t reg, uint8_t val){
    uint8_t data[] = {
        static_cast<uint8_t>(reg),
        static_cast<uint8_t>(val)
    };
    bus.write(address, data, sizeof(data) / sizeof(uint8_t));
}

void qmc5883l::init(){
    writeReg(reset1, 0x01);
    setMode(configContinue, config10hz,config8gauss, configOS512);
    writeReg(config2, 0x41);
        uint8_t data1[7] = {
        static_cast<uint8_t>(xLSB)
    };
//    uint8_t data2[7] = {};
    bus.write(address, data1, 1);
    bus.read(address, data1, 6);
    int16_t x = data1[0];
    x |= data1[1] << 8;
    int16_t y = data1[2];
    y |= data1[3] << 8;
    int16_t z = data1[4];
    z |= data1[5] << 8;
    
    xMin = xMax = x;
    yMin = yMax = y;
    zMin = zMax = z;
    
}

void qmc5883l::setMode(uint16_t mode,uint16_t odr,uint16_t rng,uint16_t osr){
    writeReg(config, mode|odr|rng|osr);
}

void qmc5883l::readV(int16_t* x,int16_t* y,int16_t* z){
    uint8_t data1[7] = {
        static_cast<uint8_t>(xLSB)
    };
//    uint8_t data2[7] = {};
    bus.write(address, data1, 1);
    bus.read(address, data1, 6);
    *x = data1[0];
    *x |= data1[1] << 8;
    *y = data1[2];
    *y |= data1[3] << 8;
    *z = data1[4];
    *z |= data1[5] << 8;
    updateVals(*x, *y, *z);
} 

void qmc5883l::calibratedVals(int16_t* x,int16_t* y,int16_t* z){
    readV(x, y, z);
    
    float offsetX = (xMax + xMin)/2;
    float offsetY = (yMax + yMin)/2;
    float offsetZ = (zMax + zMin)/2;
    
    float dX = (xMax - xMin)/2;
    float dY = (yMax - yMin)/2;
    float dZ = (zMax - zMin)/2;
    
    float ad = (dX + dY + dZ) /3;
    
    float scaleX = ad / dX;
    float scaleY = ad / dY;
    float scaleZ = ad / dZ;
    
    *x = static_cast<int16_t>((*x - offsetX) * scaleX);
    *y = static_cast<int16_t>((*y - offsetY) * scaleY);
    *z = static_cast<int16_t>((*z - offsetZ) * scaleZ);
}

float qmc5883l::azimuth(int16_t y, int16_t x){
    float a = atan2(y, x) * 180.0/Pi;
    return a <0?360 + a:a;
}

float qmc5883l::heading(int16_t* x,int16_t* y,int16_t* z){
    readV(x, y, z);
    
    float fX = static_cast<float>(*x) - (xMax+xMin)/2;
    float fY = static_cast<float>(*y) - (yMax+yMin)/2;
    hwlib::cout << "x val: " << *x << " min val: " << xMin << " max val: " << xMax << " offset: " << (xMax+xMin)/2 << " fX: " << static_cast<int>(fX) << hwlib::endl;
    hwlib::cout << "y val: " << *y << " min val: " << yMin << " max val: " << yMax << " offset: " << (yMax+yMin)/2  << " fY: " << static_cast<int>(fY) << hwlib::endl;
    fX = fX/(xMax-xMin);
    fY = fY/(yMax-yMin);
//    hwlib::cout << " fX: " << static_cast<int>(fX);
    
    float h = 180.0*atan2(fX, fY)/Pi;
    
    
    if(h<=0) h += 360.0;
    hwlib::cout << " heading: " << static_cast<int>(h);

    return h;
}

int8_t qmc5883l::readI(uint8_t reg){
    uint8_t data[1] = {
        reg
    };
    bus.write(address, data, 1);
    bus.read(address, data, 1);
    return data[0];
}

void qmc5883l::updateVals(int16_t x, int16_t y,int16_t z){
    if(x < xMin){xMin = x;}
    if(x > xMax){xMax = x;}
    if(y < yMin){yMin = y;}
    if(y > yMax){yMax = y;}
    if(z < zMin){zMin = z;}
    if(z > zMax){zMax = z;}
}

int16_t qmc5883l::getXmin(){return xMin;}
int16_t qmc5883l::getXmax(){return xMax;}
int16_t qmc5883l::getYmin(){return yMin;}
int16_t qmc5883l::getYmax(){return yMax;}