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
    setMode(configContinue, config200hz,config8gauss, configOS512);
    writeReg(config2, 0x41);
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
}

int8_t qmc5883l::readI(uint8_t reg){
    uint8_t data[1] = {
        reg
    };
    bus.write(address, data, 1);
    bus.read(address, data, 1);
    return data[0];
}