#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <cmath> 
#include <ctime>
#include <iomanip>
#include "loadData.h"

using namespace std;

unsigned char homeToBytes(int num){
    unsigned char bytes = static_cast<unsigned char>(num);
    return bytes;
}

int bytesToHome(unsigned char num){
    int out = (int)num;
    return out;
}

unsigned short int floatToBytes(double num){
    //at most 3 s.f. 
    unsigned short int bytes;
    // First bit indicates if 1 or 0. Remaining 7 bits are for decimal. (2B)
    if (num>=1){
        num = num-1;
        num = num*1000 + pow(2, 15); // put the MSB as 1 if the before decimal is 1
    }else{
        num = num*1000;
    }
    bytes = static_cast<unsigned short int>(num);
    return bytes;
}

float bytesToFloat(unsigned short int num){
    float out =0 ;
    if (num >=32768){
        num -= 32768;
        out += 1;
    }
    out = out + static_cast<float>(num)/1000;
    return out;
}

short int dateToBytes(string date){
    tm t = {};
    istringstream ss(date);
    if (ss >> get_time(&t, "%d/%m/%Y"))
    {
        short int days = (mktime(&t))/86400 + 1;
        return days;
    }
    else
    {
        cout << "Parse failed\n";
        return 0;
    }
}

string bytesToDate(int days){
    tm* time;
    char new_date[11];

    time_t date_seconds = static_cast<time_t>(days*86400);
    time = gmtime(&date_seconds);
    strftime(new_date, sizeof(new_date), "%d/%m/%Y", time);
    return string(new_date);
}

vector<Record> loadData(){

    string data;
    string field;
    vector<vector<string> > recordArr;


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
    vector<Record> recordBytes;

    for (int i=1; i<recordArr.size(); i++){
        Record r;
        short int missing = 0;
        
        r.gameDateEst = dateToBytes(recordArr[i][0]); //correct
        // cout<< "date: " + recordArr[i][0] << " --> " << r.gameDateEst << endl;

        
        r.teamIdHome = stoi(recordArr[i][1]);
        // cout<< "ID: " << recordArr[i][1] << " --> " << r.teamIdHome << endl;
    
        try{
            // r.ptsHome = intToBytes(stoi(recordArr[i][2]));
            r.ptsHome = static_cast<unsigned char>(stoi(recordArr[i][2]));
        }catch(const exception& e){
            r.ptsHome = static_cast<unsigned char>(0);
            missing += pow(2,2); //the index 4 of the 9 bits will be 1
        }
        // cout<< "PTS: " << recordArr[i][2] << " --> " << r.ptsHome << endl;
        
        try{
            r.fgPctHomeByteArray = floatToBytes(stod(recordArr[i][3]));
        }catch(const exception& e)
        {
            r.fgPctHomeByteArray = floatToBytes(0.000);
            missing += pow(2,3);
        }
        // cout<< "FG: " << recordArr[i][3] << " --> " << r.fgPctHomeByteArray << endl;

        try{
            r.ftPctHomeByteArray = floatToBytes(stod(recordArr[i][4]));
        }
        catch(const exception& e)
        {
            r.ftPctHomeByteArray = floatToBytes(0.000);
            missing += pow(2,4);
        }
        // cout<< "FT: " << recordArr[i][4] << " --> " << r.ftPctHomeByteArray << endl;
        
        try{
            r.fg3PctHomeByteArray = floatToBytes(stod(recordArr[i][5]));
        }
        catch(const exception& e)
        {
            r.fg3PctHomeByteArray = floatToBytes(0.000);
            missing += pow(2,5);
        }
        // cout<< "FG3: " << recordArr[i][5] << " --> " << r.fg3PctHomeByteArray << endl;
        
        try{
            r.astHome = static_cast<unsigned char>(stoi(recordArr[i][6]));
        }
        catch(const exception& e)
        {
            r.astHome = static_cast<unsigned char>(0);
            missing +=pow(2,6);
        }
        // cout<< "AST: " << recordArr[i][6] << " --> " << r.astHome << endl;
        // cout << bytesToHome(r.astHome) << endl;

        try{
            r.rebHome = static_cast<unsigned char>(stoi(recordArr[i][7]));
        }
        catch(const exception& e)
        {
            r.rebHome = static_cast<unsigned char>(0);
            missing += pow(2,7);
        }
        // cout<< "REB: " << recordArr[i][7] << " --> " << r.rebHome << endl;
        // cout << bytesToHome(r.rebHome) << endl;
        
        r.homeTeamWins= (bool)(stoi(recordArr[i][8]));
        // cout<< "win: " << r.homeTeamWins << endl;

        r.recorderHeader = missing;
        
        // cout<<"Empty fields: "<<r.recorderHeader<<endl;
        // cout<< "bytesize = "<< sizeof(r) << endl;
        // cout << endl;
    
        recordBytes.push_back(r);
        
        }
    return recordBytes;
};