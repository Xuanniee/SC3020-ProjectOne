#ifndef LOAD_DATA
#define LOAD_DATA

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <cmath> 
#include <ctime>
#include "./Record/record.h"

#define NUM_FIELDS 9

//conversion helper functions
unsigned char homeToBytes(int num);

int bytesToHome(unsigned char num);

unsigned short int floatToBytes(double num);

float bytesToFloat(unsigned short int num);

short int dateToBytes(std::string date);

std::string bytesToDate(int days);

std::vector<Record> loadData();

#endif