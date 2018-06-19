#include "hwlib.hpp"
#include "qmc5883l.hpp"
int main( void ){	
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda1);
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl1);
    auto i2cBus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    auto qmc = qmc5883l(i2cBus, 0x0D);
    qmc.init();
//    int8_t x ;
    int16_t x,y,z;
    while(1){
        qmc.readV(&x, &y, &z);
//        x = qmc.readI(0x05);
//        hwlib::cout << "x: " << x << hwlib::endl;
        hwlib::cout << "x: " << x << " y: " << y << " z: " << z << hwlib::endl;
    }
    hwlib::wait_ms(100);
}
