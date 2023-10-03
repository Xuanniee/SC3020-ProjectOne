#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <cmath> 

#define MAX_RECORDS 19
#define MAX_BLOCKS 1250000
#define NUM_FIELDS 9

unsigned char intToBytes(int num);
unsigned char* floatToBytes(float num);
int dateToBytes(string date);



struct record{
    short int recorderHeader;
    unsigned char rebHome;

    bool homeTeamWins;
    // Field Headers for the 9 Attributes
    // 3B is enough to represents all the dates we need but 4B for ease
    int gameDateEst; 

    // Similar format to gameDateEst, so use same data size
    int teamIdHome; 

    // Integer by nature of data
    unsigned char ptsHome;

    // Primary Key; Float; Uses 2B
    unsigned char fgPctHomeByteArray[2];

    // Same reason as above
    unsigned char ftPctHomeByteArray[2];

    // Same reason as above
    unsigned char fg3PctHomeByteArray[2];

    // Same as ptsHome
    unsigned char astHome;

};

struct dataBlock {
    int numRecords;
    record recordBytes[MAX_RECORDS];
};


int main();
