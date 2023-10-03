#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <cmath> 

using namespace std;

unsigned char* intToBytes(int num, int sizeByte);

unsigned char* floatToBytes(float num, size_t sizeByte);

unsigned char* boolToBytes(bool num, size_t sizeByte);

unsigned char* dateToBytes(string date, int sizeByte);