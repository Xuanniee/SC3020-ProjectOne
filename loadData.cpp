#include <iostream>
#include <fstream>
// #include <cstring>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <cmath> 

using namespace std;

unsigned char* intToBytes(int num, int sizeByte){
    unsigned char* bytes = new unsigned char[sizeByte]; //can use sizeof num?
    for (int i=0; i<sizeByte; i++){
        bytes[i] = (num >> ((sizeByte-1-i)*8)) & 0xFF;
    }
    return bytes;
}

unsigned char* floatToBytes(float num, size_t sizeByte = 2){
    //at most 3 s.f.
    unsigned char* bytes = new unsigned char[sizeByte];
    
    // First bit indicates if 1 or 0. Remaining 7 bits are for decimal. (2B)
    if (num>=1){
        num = num-1;
        int val = num*1000 + pow(2, 15); // put the MSB as 1 if the before decimal is 1
        for (int i=0; i<sizeByte; i++){
            bytes[i] = (val >> ((sizeByte-1-i)*8)) & 0xFF;
        }
    }else{
        int val = num*1000;
        for (int i=0; i<sizeByte; i++){
            bytes[i] = (val >> ((sizeByte-1-i)*8)) & 0xFF;
        }
    }
    return bytes;
}

unsigned char* boolToBytes(bool num, size_t sizeByte = 1){
    unsigned char* bytes = new unsigned char[sizeByte];
    bytes[0] = num & 0xFF;
    return bytes;
}

unsigned char* dateToBytes(string date, int sizeByte = 10){
    unsigned char* bytes = new unsigned char[sizeByte];
    //convert to DD/MM/YYYY format
    if (date.length() < 10){
        if (date[1] == '/'){
            date.insert(0, 1, '0');
            }
        if (date[4] == '/'){
            date.insert(3, 1, '0');
            }
    }
    for (int i=0; i<sizeByte; i++){
        bytes[i] = static_cast<unsigned char>(date[i]);
        // cout << bytes[i] <<endl;
    }
    return bytes;
}
 
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


int main(){

    const int NUMFIELDS = 9;
    vector<vector<string> > recordArr;
    string data;
    string field;

    //read in file
    string textfile = "games.txt";
    ifstream MyReadFile(textfile);
    while(getline(MyReadFile, data)){
        stringstream linestream(data);
        vector<string> rec;
        while(getline(linestream, field, '\t')){
            rec.push_back(field);
        }
        recordArr.push_back(rec);
    }
    MyReadFile.close();
    
    //store in bytes format
    vector<record> recordBytes;

    for (int i=1; i<recordArr.size(); i++){
        record r;
        int field = 0;
        string missing;
        
        // r.date = dateToBytes(recordArr[i][0]); 
        // cout<< r.date << endl;

        
        // r.teamID = intToBytes(stoi(recordArr[i][1]), 4);
        // cout<< r.teamID << endl;
       
        // try{
        //     r.PTS = intToBytes(stoi(recordArr[i][2]), 1);
        // }catch(const exception& e){
        //     r.PTS = intToBytes(0, 1);
        //     missing.push_back('2');
        // }
        
        // try{
        //     r.FG_PCT = floatToBytes(stof(recordArr[i][3]));
        //     cout<< r.FG_PCT << endl;
        // }catch(const exception& e)
        // {
        //     r.FG_PCT = floatToBytes(0.000);
        //     missing.push_back('3');

        // }

        // try{
        //     r.FT_PCT = floatToBytes(stof(recordArr[i][4]));
        //     cout<< r.FG_PCT << endl;
        // }
        // catch(const exception& e)
        // {
        //     r.FT_PCT = floatToBytes(0.000);
        //     missing.push_back('4');
        // }
        
        // try{
        //     r.FG3 = floatToBytes(stof(recordArr[i][5]));
        //     cout<< r.FG3 << endl;
        // }
        // catch(const exception& e)
        // {
        //     r.FG3 = floatToBytes(0.000);
        //     missing.push_back('5');
        // }
        
        // try{
        //     r.AST = intToBytes(stoi(recordArr[i][6]),1);
        //     cout<< r.AST << endl;
        // }
        // catch(const exception& e)
        // {
        //     r.AST = intToBytes(0,1);
        //     missing.push_back('6');
        // }
        
        // try{
        //     r.REB = intToBytes(stoi(recordArr[i][7]),1);
        //     cout<< r.REB << endl;
        // }
        // catch(const exception& e)
        // {
        //     r.REB = intToBytes(0,1);
        //     missing.push_back('7');
        // }

        
        // bool temp = (bool)(stoi(recordArr[i][8]));
        // r.win = boolToBytes(temp);
        // cout<< temp << endl;

        // r.header = new unsigned char[missing.length()];
        // for (int j=0; j<missing.length(); j++){
        //     r.header[j] = static_cast<unsigned char>(missing[j]);
        // }
        
        // cout<<"Empty fields: "<<r.header<<endl;
        cout<< "bytesize = "<< sizeof(int[2]) << endl;
    
        // recordBytes.push_back(r);
        }
         

};

//check for the conversion 
//have a reverse conversion