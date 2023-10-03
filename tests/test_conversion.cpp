#include "../loadData.h"


/**
 * @brief Compare if an array of unsigned char array is composed of specified bits
 *
 * @param bytes Array of unsigned char that represent bytes
 * @param tgt   Array of 0s and 1s that the array of bytes is meant to be equal to
 * @return Whether bytes matches tgt
 */
bool _cmp_bytes(unsigned char *bytes, vector<bool> tgt)
{
    int ib=0, it=0;
    unsigned char byte;
    bool bit;

    // tgt.size() must be multiples of 8
    if (tgt.size() % 8 != 0 || !bytes) return false;

    while ((byte = bytes[ib++]) && it < tgt.size()) {
        for (int _ = 0; _ < 8; _++) {
            bit = 128 & byte;
            byte <<= 1;
            if (tgt[it++] != bit) return false;
        }
    }
    return true;
}


/**
 * @brief Test conversion of 8 bit ints to 1 unsigned char
 */
void test_int_to_bytes()
{
    int v;
    vector<bool> bits;
    unsigned char* bytes;

    // Test 1.1
    v = 0;
    bits = {0,0,0,0,0,0,0,0};
    bytes = intToBytes(v, (int) 1);
    cout << (_cmp_bytes(bytes, bits) ? "[SUCCESS]" : "[FAILED]") << " Test 1.1: Convert " << v << " to bytes " << endl;

    // Test 1.2
    v = 1;
    bits = {0,0,0,0,0,0,0,1};
    bytes = intToBytes(v, (int) 1);
    cout << (_cmp_bytes(bytes, bits) ? "[SUCCESS]" : "[FAILED]") << " Test 1.2: Convert " << v << " to bytes " << endl;

    // Test 1.3
    v = 130;
    bits = {1,0,0,0,0,0,1,0};
    bytes = intToBytes(v, (int) 1);
    cout << (_cmp_bytes(bytes, bits) ? "[SUCCESS]" : "[FAILED]") << " Test 1.3: Convert " << v << " to bytes " << endl;

    // Test 1.4
    v = 255;
    bits = {1,1,1,1,1,1,1,1};
    bytes = intToBytes(v, (int) 1);
    cout << (_cmp_bytes(bytes, bits) ? "[SUCCESS]" : "[FAILED]") << " Test 1.4: Convert " << v << " to bytes " << endl;
}


/**
 * @brief Test conversion of 8 bit bools to 1 unsigned char
 */
void test_bool_to_bytes() {}


/**
 * @brief Run all tests
*/
void test_conversion() {
    test_int_to_bytes();
    test_bool_to_bytes();
}