#include <cstring>
#include "../loadData.h"


bool test_date_to_bytes() {
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


/**
 * @brief Run all tests
*/
void test_conversion() {
    test_date_to_bytes();
}

//g++ test.cpp loadData.cpp -o test; .\test

//g++ main.cpp loadData.cpp Node/node.cpp BPlusTree/utils.cpp BPlusTree/bPlusTree.cpp "Block Manager/blockManager.cpp" Record/record.h -o main