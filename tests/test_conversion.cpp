#include <cstring>
#include "../loadData.h"


void test_date_to_bytes() {
    short bytes;
    std::string odate, idate;
    
    idate = "22/12/2022";
    bytes = dateToBytes(idate);
    odate = bytesToDate(bytes);
    std::cout << (idate.compare(odate)==0 ? "[PASSED]" : "[FAILED]") << " assert "<< idate << " == " << odate << std::endl;

    idate = "23/12/2022";
    bytes = dateToBytes(idate);
    odate = bytesToDate(bytes);
    std::cout << (idate.compare(odate)==0 ? "[PASSED]" : "[FAILED]") << " assert "<< idate << " == " << odate << std::endl;
}


void test_float_to_bytes() {
    short bytes;
    float ifloat, ofloat;

    ifloat = 0.0;
    bytes = floatToBytes(ifloat);
    ofloat = bytesToFloat(bytes);
    std::cout << (ifloat == ofloat ? "[PASSED]" : "[FAILED]") << " assert "<< ifloat << " == " << ofloat << std::endl;

    ifloat = 0.499;
    bytes = floatToBytes(ifloat);
    ofloat = bytesToFloat(bytes);
    std::cout << (ifloat == ofloat ? "[PASSED]" : "[FAILED]") << " assert "<< ifloat << " == " << ofloat << std::endl;

    ifloat = 0.511;
    bytes = floatToBytes(ifloat);
    ofloat = bytesToFloat(bytes);
    std::cout << (ifloat == ofloat ? "[PASSED]" : "[FAILED]") << " assert "<< ifloat << " == " << ofloat << std::endl;

    ifloat = 0.501;
    bytes = floatToBytes(ifloat);
    ofloat = bytesToFloat(bytes);
    std::cout << (ifloat == ofloat ? "[PASSED]" : "[FAILED]") << " assert "<< ifloat << " == " << ofloat << std::endl;

    ifloat = 2.0;
    bytes = floatToBytes(ifloat);
    ofloat = bytesToFloat(bytes);
    std::cout << (ifloat == ofloat ? "[PASSED]" : "[FAILED]") << " assert "<< ifloat << " == " << ofloat << std::endl;
}


/**
 * @brief Run all tests
*/
void test_conversion() {
    // test_date_to_bytes();
    test_float_to_bytes();
}

//g++ test.cpp loadData.cpp -o test; .\test

//g++ main.cpp loadData.cpp Node/node.cpp BPlusTree/utils.cpp BPlusTree/bPlusTree.cpp "Block Manager/blockManager.cpp" Record/record.h -o main