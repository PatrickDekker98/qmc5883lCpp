#include "hwlib.hpp"
#include "qmc5883l.hpp"
#include <math.h>
int main( void ){	
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda1);
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl1);
    auto sda1 = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto scl1 = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto i2cBus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    auto i2cBus2 = hwlib::i2c_bus_bit_banged_scl_sda( scl1,sda1 );
    auto qmc = qmc5883l(i2cBus, 0x0D);
    auto display = hwlib::glcd_oled( i2cBus2, 0x3c );
    display.clear();
    auto l = hwlib::line( hwlib::location(128, 32), hwlib::location(96, 32) );
    l.draw(display);
    qmc.init();
    float a ;
    float azi;
    float b ;
    float ox ;
    float oy ;
    int16_t x,y,z;
    while(1){
  //  for(unsigned int i =0; i < 100 ; i++)
        display.clear();
        a = qmc.heading(&x, &y, &z);
        azi = qmc.azimuth(x, y);
        b = a * qmc.Pi / 180;
        ox = -32 * cos(b);
        oy = -32 * sin(b);
        l = hwlib::line( hwlib::location(static_cast<int>(ox)+32, static_cast<int>(oy)+32), hwlib::location(32, 32) );
        l.draw(display);
//        hwlib::cout << "x: " << x << " y: " << y << " z: " << z << hwlib::endl;
        hwlib::cout << "heading: " << static_cast<int16_t>(a) << hwlib::endl;
//        hwlib::cout << "azimuth: " << static_cast<int16_t>(azi) << hwlib::endl;

    }
    hwlib::wait_ms(100);
    hwlib::cout <<  "xmin: "<< qmc.getXmin() <<  " xmax: "<<  qmc.getXmax() <<  " ymin: "<< qmc.getYmin() <<  " ymax: "<< qmc.getYmax() << hwlib::endl; 
}
