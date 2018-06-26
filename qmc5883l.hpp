// ==========================================================================
//
// File      : qmc5883l.hpp
// Part of   : qmc5883l magnetic compass library 
// Copyright : Patrick_Dekker@hotmail.com 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// this file contains Doxygen lines
/// @file

#ifndef QMC5883L_HPP
#define QMC5883L_HPP

#include "hwlib.hpp"
#include <array>
#include <math.h>
/// \brief
/// abstract magnetometer class
/// \details
/// This is an abstract class for a library witch reads and controlls a magnetometer.
/// only i2c magnetometers with 8bit registers like the qmc5883l and the hmc5883l are supported.
class magnetometer {
public:
    virtual void init()=0;
    virtual void setMode(uint16_t mode,uint16_t rate,uint16_t range,uint16_t osr)=0;
    virtual void readV(int16_t* x,int16_t* y,int16_t* z)=0;
    virtual int8_t readI(uint8_t reg)=0;
//    virtual void setDeclination()=0;

};
/// \brief
/// qmc5883l magnetometer implementation 
/// \details
/// This class implements a interface for the qmc5883l magnetometer module.
class qmc5883l : public magnetometer{
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

    /// \brief
    /// construct a qmc5883l class from a I2C bus and an address
    /// \details
    /// This constructor creates a class for the qmc5883l chip.
    /// Requires a I2C bus and a I2C address.
    /// If no address is provided 0x0d is assumed.
    qmc5883l(hwlib::i2c_bus & bus, const uint_fast8_t & address = 0x0d);
    
    float Pi = 3.14159265358979323846264338327950288;

    /// \brief
    /// initialise the chip
    /// \details
    /// This function first writes to the reset register to reset it.
    /// then it configures the modes the chip opperates on.
    /// last it does initial readings and sets the minimal and maximal values.
    void init()override;
//    void setDeclination()override;

    /// \brief
    /// sets configure options for the chip 
    /// \details
    /// It writes to the config register with the config bits.
    /// Each two bits set another configuration.
    /// Mode sets the chip to standby or continous mode, range to 4 different update rates, range 2 different ranges, and osr to 4 different oversampling rates.
    void setMode(uint16_t mode,uint16_t rate,uint16_t range,uint16_t osr)override;

    /// \brief
    /// readV (read values) reads all values of the magnetometer
    /// \details
    /// This function reads all magnetometer axis registers starting with X LSB register and ending with Z MSB register.
    void readV(int16_t* x,int16_t* y,int16_t* z)override;
    
    /// \brief
    /// calibratedVals calibrates all values.
    /// \details
    /// This value first calls readV.
    /// It then proceeds to calibrate the values and attempts to remove hard iron distortion and soft iron distortion.
    /// more information here ===> https://appelsiini.net/2018/calibrate-magnetometer/
    void calibratedVals(int16_t* x, int16_t* y,int16_t* z);
    
    /// \brief
    /// readI (read inividual) reads an inividual register.
    /// \details
    /// This function reads a single inividual register.
    /// It then proceeds to return a single inividual value..
    int8_t readI(uint8_t reg)override;
    
    /// \brief
    /// azimuth returns the azimuth value
    /// \details
    /// This function returns the azimuth value.
    /// In this case it is an absolute bearing not relative.
    float azimuth(int16_t y, int16_t x);
    
    /// \brief
    /// heading returns the heading value
    /// \details
    /// this function returns the heading value.
    /// The heading is relative to magnetic north.
    /// a heading of 90 degrees would mean the magnetometer is pointing east granted there is no significant distrotion.
    float heading(int16_t* x,int16_t* y,int16_t* z) ;
    
    /// \brief
    /// heading returns the heading value but in radians
    /// \details
    /// this function returns the heading value in radians.
    /// The heading is relative to magnetic north.
    /// a heading of 1/2pi radians would mean the magnetometer is pointing east granted there is no significant distrotion.
    float headingRad(int16_t* x,int16_t* y,int16_t* z);
    
    
    void getTemp(int16_t* t);

    float returnTemp();

};

#endif // QMC5883L_HPP
