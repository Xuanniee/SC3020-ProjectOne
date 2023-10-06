#ifndef RECORD
#define RECORD
//record structure
typedef struct record{
    
    //9 bits for all 9 fields (0-9)
    short int recorderHeader;

    //index 0 : number of days since epoch
    short int gameDateEst;

    //index 1: range is within 4bytes 
    int teamIdHome;

    // index 2: range is within 1byte
    unsigned char ptsHome;

    //index 3: 3 decimals(0 to 999) + MSB of 1/0
    unsigned short int fgPctHomeByteArray;

    //index 4: 3 decimals(0 to 999) + MSB of 1/0
    unsigned short int ftPctHomeByteArray;

    //index 5: 3 decimals(0 to 999) + MSB of 1/0
    unsigned short int fg3PctHomeByteArray;

    //index 6: range is within 1byte
    unsigned char astHome;

    //index 7: range is within 1byt
    unsigned char rebHome;

    //index 8: range is within 1byte
    bool homeTeamWins;
} Record;

#endif