#include "hwlib.hpp"
#include "qmc5883l.hpp"
//#include "test.hpp"
#include <math.h>

void startUp(hwlib::window_ostream & console){
    console << "\f" << "compas is " << "\n" << "starting " << "\n" << "please calibrate " << "\n" <<  "the compass " << "\n" << "by turning it " << "\n" <<"around " << hwlib::flush;
    hwlib::wait_ms(1000);
}

void drawCompass(hwlib::window & display){
    auto c = hwlib::circle(hwlib::location(32, 32), 25);
    auto l1 = hwlib::line(hwlib::location(32, 15), hwlib::location(32, 0));
    auto l2 = hwlib::line(hwlib::location(0 , 32), hwlib::location(15 , 32));
    auto l3 = hwlib::line(hwlib::location(32, 49), hwlib::location(32, 64));
    auto l4 = hwlib::line(hwlib::location(49, 32), hwlib::location(64, 32));
    c.draw(display);
    l1.draw(display);
    l2.draw(display);
    l3.draw(display);
    l4.draw(display);
    
}

int main( void ){	
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda1 = hwlib::target::pin_oc(hwlib::target::pins::sda1);
    auto scl1 = hwlib::target::pin_oc(hwlib::target::pins::scl1);
    auto i2cBus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    auto i2cBus2 = hwlib::i2c_bus_bit_banged_scl_sda( scl1,sda1 );
//    hwlib::cout<< "what? ";
//    runTests(i2cBus2);
    auto qmc = qmc5883l(i2cBus2, 0x0D);
    auto display = hwlib::glcd_oled( i2cBus, 0x3c );
    auto font = hwlib::font_default_8x8();
    auto bigConsole = hwlib::window_ostream(display, font);
    startUp(bigConsole);
    auto roset = hwlib::window_part(display, hwlib::location(0, 0), hwlib::location(64, 64));
    auto consDisp = hwlib::window_part(display, hwlib::location(64, 0), hwlib::location(128, 64));
    auto console = hwlib::window_ostream(consDisp, font);
    display.clear();
    hwlib::line l = hwlib::line( hwlib::location(64, 32), hwlib::location(32, 32) );
    l.draw(roset);
    hwlib::line oldL = hwlib::line( hwlib::location(64, 32), hwlib::location(32, 32), hwlib::white );
    qmc.init();
    float a ;
    float h;
//    float t ;
    float azi;
//    float b ;
    float ox ;
    float oy ;
    int16_t x,y,z;
    while(1){
  //  for(unsigned int i =0; i < 100 ; i++)
//        display.clear();
        a = qmc.heading(&x, &y, &z);
        h = qmc.headingDeg(&x, &y, &z);
//        t = qmc.returnTemp();
        a += qmc.Pi/2;
        h += 90;
        if(h >= 360) {
            h-=360;
        }
        azi = qmc.azimuth(x, y);
        ox = -31 * cos(a);
        oy = -31 * sin(a);
        oldL.draw(roset);
        l = hwlib::line( hwlib::location(static_cast<int>(oy)+32, static_cast<int>(ox)+32), hwlib::location(32, 32) );
        oldL = hwlib::line( hwlib::location(static_cast<int>(oy)+32, static_cast<int>(ox)+32), hwlib::location(32, 32), hwlib::white); 
        drawCompass(roset);
        l.draw(roset);
        console << "\f" << static_cast<int>(h) << "\n" << hwlib::flush;
//        console << "\f" << "attempted heading " << "\n" << hwlib::flush;
//        roset.clear();
//        hwlib::cout << "x: " << x << " y: " << y << " z: " << z << hwlib::endl;
//        hwlib::cout << "heading: " << static_cast<int16_t>(a) << hwlib::endl;
//        hwlib::cout << "azimuth: " << static_cast<int16_t>(azi) << hwlib::endl;
        hwlib::wait_ms(10);

    }
    //hwlib::wait_ms(100);
}
