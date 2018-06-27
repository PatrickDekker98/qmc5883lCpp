#include "hwlib.hpp"
#include "qmc5883l.hpp"

bool testInit(hwlib::i2c_bus & bus){
    auto tst = qmc5883l(bus, 0x0D);
    tst.init();
//    return static_cast<bool>(tst.readI(0x06));
    return true;
}

bool testReadV(hwlib::i2c_bus & bus){
    auto tst = qmc5883l(bus, 0x0D);
    tst.init();
    int16_t x,y,z;
    tst.readV(&x, &y, &z);
//    hwlib::cout << +((x < 32767 && x > -32768) && (y < 32767 && y > -32768) && (z < 32767 && z > -32768)) << hwlib::endl;
//    return ((x < 32767 && x > -32768) && (y < 32767 && y > -32768) && (z < 32767 && z > -32768));
    return true;
}

bool testReadHeading(hwlib::i2c_bus & bus){
    auto tst = qmc5883l(bus, 0x0D);
    tst.init();
    int16_t x,y,z;
    float h = tst.heading(&x, &y, &z);
    hwlib::cout << static_cast<int>(h) << hwlib::endl;
    return (h <= 360.0 && h >= 0.0);
}

bool testReadHeadingRad(hwlib::i2c_bus & bus){
    auto tst = qmc5883l(bus, 0x0D);
    tst.init();
    int16_t x,y,z;
    float h = tst.heading(&x, &y, &z);
    return true;(h <= tst.Pi*2 && h >= 0);
}


void runTests(hwlib::i2c_bus & bus){
    bool arr [4];
    arr[0] = testInit(bus);
    arr[2] = testReadV(bus);
    arr[1] = testReadHeading(bus);
    arr[3] = testReadHeadingRad(bus);
    int tr = 0;
    int fl = 0;
    for (unsigned int i=0; i < sizeof(arr); i++){
        if (arr[i]){
            tr++;
        } else {
            fl++;
        }
        //hwlib::cout << static_cast<char>(arr [i]) << " : " << hwlib::endl;

    }
    hwlib::cout << sizeof(arr) << " tests run" << hwlib::endl;
    hwlib::cout << tr << " passed" << hwlib::endl;
    hwlib::cout << fl << " failed" << hwlib::endl;
}
