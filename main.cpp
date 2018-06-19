#include "hwlib.hpp"
#include "aMagnetoMeter.hpp"
namespace  target = hwlib::target;



int main( void ){	
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    auto drdy = target::pin_in(target::pins::d8);
    auto scl = target::pin_oc(target::pins::scl);
    auto sda = target::pin_oc(target::pins::sda);
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
    auto m = aMagnetoMeter(i2c_bus, 0x0d);
    m.init();
//    uint_fast8_t x ;
//    uint_fast8_t reg = 0x01 ;
//    auto n = 1;
//    float a;
    std::array<int16_t, 3> x;
    std::array<bool, 3> status;
//    std::array<int16_t, 3> vals;
    for (unsigned int i =0; i < 1000; i++){
//    while(1){
        status = m.ready();
//        hwlib::cout << status[0] << "< drdy .. ovl >" << status[1] << " dor >>" << status[2] << hwlib::endl;
        if(status[0] && !status[1]){
//            x = m.readReg(reg, n);
            x = m.readDirs();
            hwlib::cout <<  "x: "<< x[0] << " y: " << x[1] << " z: " << x[2];
//            hwlib::cout << x << hwlib::endl;
//            a = m.azimuth(x[1], x[0]);
//            hwlib::cout << "   azimuth allegedly: " << static_cast<int>(a) << hwlib::endl;

        } else {
            hwlib::cout << "wrong" << hwlib::endl;
        }
        hwlib::wait_ms(100);
    }
    hwlib::cout <<  "xmin: "<< m.getXmin() <<  " xmax: "<<  m.getXmax() <<  " ymin: "<< m.getYmin() <<  " ymax: "<< m.getYmax() << hwlib::endl;
}
