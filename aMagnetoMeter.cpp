#include <array>
#include "aMagnetoMeter.hpp"

aMagnetoMeter::aMagnetoMeter(hwlib::i2c_bus & bus, const uint_fast8_t & address):
    bus(bus),
    address(address)
{}

void aMagnetoMeter::writeReg(uint_fast8_t & reg, uint_fast8_t d){
    uint8_t data[] = {
        static_cast<uint8_t>(reg),
        static_cast<uint8_t>(d)
    };
    bus.write(address, data, sizeof(data) / sizeof(uint8_t));
}

uint8_t aMagnetoMeter::readReg(uint_fast8_t & reg, size_t n){
    uint8_t data1[] = {
        static_cast<uint8_t>(reg)
    };
    bus.write(address, data1, 1);
//    static_cast<int8_t>(data1);
//    uint8_t x = 0x00;
//    uint8_t data2[] = {x};
    bus.read(address, data1, n);
    return static_cast<uint8_t>(data1[0]);
}

std::array<int16_t, 3> aMagnetoMeter::readDirs(){
    uint8_t data1[8] = {
        static_cast<uint8_t>(xLSB)
    };
    bus.write(address, data1, 1);
    bus.read(address, data1, 7);
    int16_t x = 0;
    int16_t y =0;
    int16_t z =0;
    x = static_cast<int16_t>(data1[1] <<8  | data1[0]);
    y = static_cast<int16_t>(data1[3] <<8  | data1[2]);
    z = static_cast<int16_t>(data1[5] <<8  | data1[4]);
    
//    hwlib::cout << data1[0] << " <0: " << data1[1] << " <1: " << data1[2] << " <2: " << data1[3] << " <3: " << data1[4] << " <4: " << data1[5] << " <5: " << data1[6] << " <6: " << hwlib::endl;
    
    if(x < xMin){xMin = x;}
    if(x > xMax){xMax = x;}
    if(y < yMin){yMin = y;}
    if(y > yMax){yMax = y;}
    if(z < zMin){zMin = z;}
    if(z > zMax){zMax = z;}
/*    
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
    
    x = static_cast<int16_t>((x - offsetX) * scaleX);
    y = static_cast<int16_t>((y - offsetY) * scaleY);
    z = static_cast<int16_t>((z - offsetZ) * scaleZ);*/
    
    std::array<int16_t, 3> tmp = { 
        x ,
        y ,
        z 
    };

    return tmp;
}

float aMagnetoMeter::azimuth(int16_t y, int16_t x){
    float a = atan2(y, x) * (180.0/Pi);
    return a ;
}

float aMagnetoMeter::heading(int16_t y, int16_t x){
    float a = atan(y/x)* (180.0/Pi);
    return a;
}

std::array<int16_t, 3> aMagnetoMeter::readRaws(){
    std::array<int16_t, 3> data1;
    uint8_t tmp1;
    int16_t tmp2;
    for (unsigned int i=0; i < 6; i++){
        tmp1 = readReg(dirRegs[i], 1);
        if ( i % 2 != 0){
            tmp2 = 0;
            tmp2 += tmp1;
        } else {
            tmp2 += static_cast<int16_t>(tmp1) << 8;
            data1[i-1 / 2] = tmp2;
        }
    }
    if(data1[0] < xMin){xMin = data1[0];}
    if(data1[0] > xMax){xMax = data1[0];}
    if(data1[1] < yMin){yMin = data1[1];}
    if(data1[1] > yMax){yMax = data1[1];}
    return data1;
}

/*std::array<int, 3> aMagnetoMeter::returnVals(){
    std::array<int16_t, 3> vals = readRaws();
    
}*/

float aMagnetoMeter::returnX(){
    std::array<int16_t, 3> vals = readRaws();
    float x = vals[0] -(xMax+xMin)/2;
    float y = vals[1] -(yMax+yMin)/2;
    float fx = (float)x/(xMax-xMin);
    float fy = (float)y/(yMax-xMin);
    return 180.0*atan2(fy,fx)/Pi;
}

std::array<bool, 3> aMagnetoMeter::ready(){
    uint8_t x = readReg(status, 1);
    uint8_t tmp1 = x;
    uint8_t tmp2 = x;
    uint8_t tmp3 = x;

    tmp1<<=7;
    tmp1>>=7;
    tmp2<<=6;
    tmp2>>=7;
    tmp3<<=5;
    tmp3>>=7;


    std::array<bool, 3> retvals = { 
        static_cast<bool>(tmp1), 
        static_cast<bool>(tmp2), 
        static_cast<bool>(tmp3) 
    };
//    uint8_t y = 0x01;
    return retvals;
}

void aMagnetoMeter::resetChip(){
    writeReg(reset, 0x01);
}

void aMagnetoMeter::init(){
    resetChip();
    writeReg(config, oversampling|range|rate|mode);
    writeReg(config2, 0x40);
    
}

int16_t aMagnetoMeter::getXmin(){
    return xMin;
}
int16_t aMagnetoMeter::getXmax(){
    return xMax;
}
int16_t aMagnetoMeter::getYmin(){
    return yMin;
}
int16_t aMagnetoMeter::getYmax(){
    return yMax;
}



