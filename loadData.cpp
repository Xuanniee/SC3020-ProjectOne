#include <iostream>
#include <fstream>
// #include <cstring>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <cmath> 
#include <ctime>

using namespace std;

unsigned char intToBytes(int num){
    unsigned char bytes; //for the 1 byte int 
        bytes = (num >> 0) & 0xFF;
    return bytes;
}

unsigned char* floatToBytes(float num){
    //at most 3 s.f.
    int sizeOfBytes = 2;
    unsigned char* bytes = new unsigned char[sizeOfBytes];
    // First bit indicates if 1 or 0. Remaining 7 bits are for decimal. (2B)
    if (num>=1){
        num = num-1;
        int val = num*1000 + pow(2, 15); // put the MSB as 1 if the before decimal is 1
        for (int i=0; i<sizeOfBytes; i++){
            bytes[i] = (val >> ((sizeOfBytes-1-i)*8)) & 0xFF;
        }
    }else{
        int val = num*1000;
        for (int i=0; i<sizeOfBytes; i++){
            bytes[i] = (val >> ((sizeOfBytes-1-i)*8)) & 0xFF;
        }
    }
    return bytes;
}

short int dateToBytes(string date){
    tm t = {};
    istringstream ss(date);
    if (ss >> get_time(&t, "%d/%m/%Y"))
    {
        short int days = (mktime(&t))/86400;
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
 
struct record{
    // Field Headers for the 9 Attributes
    short int recorderHeader;
    
    //index 0
    unsigned char rebHome; 

    //index 1
    bool homeTeamWins; 
    
    // 3B is enough to represents all the dates we need but 4B for ease
    // index 2
    short int gameDateEst; 

    // Similar format to gameDateEst, so use same data size
    //index 3
    int teamIdHome; 

    // Integer by nature of data
    //index 4
    unsigned char ptsHome;

    // Primary Key; Float; Uses 2B
    //index 5
    unsigned char fgPctHomeByteArray[2];

    // Same reason as above
    //index 6
    unsigned char ftPctHomeByteArray[2];

    // Same reason as above
    //index 7
    unsigned char fg3PctHomeByteArray[2];

    // Same as ptsHome
    //index 8
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

    // for (int i=1; i<recordArr.size(); i++){
    for (int i=1; i<20; i++){
        record r;
        int field = 0;
        short int missing = 0;
        
        r.gameDateEst = dateToBytes(recordArr[i][0]); //correct
        cout<< "date: " + recordArr[i][0] << " --> " << r.gameDateEst << endl;

        
        r.teamIdHome = stoi(recordArr[i][1]);
        cout<< "ID: " << recordArr[i][1] << " --> " << r.teamIdHome << endl;
    
        try{
            // r.ptsHome = intToBytes(stoi(recordArr[i][2]));
            r.ptsHome = static_cast<unsigned char>(stoi(recordArr[i][2]));
        }catch(const exception& e){
            r.ptsHome = static_cast<unsigned char>(0);
            missing += pow(2,4); //the index 4 of the 9 bits will be 1
        }
        cout<< "PTS: " << recordArr[i][2] << " --> " << r.ptsHome << endl;
        
        try{
            unsigned char* temp = floatToBytes(stof(recordArr[i][3]));
            r.fgPctHomeByteArray[0] = temp[0];
            r.fgPctHomeByteArray[1] = temp[1];
            // cout<< r.fgPctHomeByteArray << endl;
        }catch(const exception& e)
        {
            unsigned char* temp = floatToBytes(0.000);
            r.fgPctHomeByteArray[0] = temp[0];
            r.fgPctHomeByteArray[1] = temp[1];
            missing += pow(2,5);
        }
        cout<< "FG: " << recordArr[i][3] << " --> " << r.fgPctHomeByteArray[0] << endl;

        try{
            unsigned char* temp = floatToBytes(stof(recordArr[i][4]));
            r.ftPctHomeByteArray[0] = temp[0];
            r.ftPctHomeByteArray[1] = temp[1];
        }
        catch(const exception& e)
        {
            unsigned char* temp = floatToBytes(0.000);
            r.ftPctHomeByteArray[0] = temp[0];
            r.ftPctHomeByteArray[1] = temp[1];
            missing += pow(2,6);
        }
        cout<< "FT: " << recordArr[i][4] << " --> " << r.ftPctHomeByteArray[0] << endl;
        
        try{
            unsigned char* temp = floatToBytes(stof(recordArr[i][5]));
            r.fg3PctHomeByteArray[0] = temp[0];
            r.fg3PctHomeByteArray[1] = temp[1];
            // cout<< r.fg3PctHomeByteArray << endl;
        }
        catch(const exception& e)
        {
            unsigned char* temp = floatToBytes(0.000);
            r.fg3PctHomeByteArray[0] = temp[0];
            r.fg3PctHomeByteArray[1] = temp[1];
            missing += pow(2,7);
        }
        cout<< "FG3: " << recordArr[i][5] << " --> " << r.fg3PctHomeByteArray[0] << endl;
        
        try{
            r.astHome = static_cast<unsigned char>(stoi(recordArr[i][6]));
        }
        catch(const exception& e)
        {
            r.astHome = static_cast<unsigned char>(0);
            missing +=pow(2,8);
        }
        cout<< "AST: " << recordArr[i][6] << " --> " << r.astHome << endl;

        try{
            r.rebHome = static_cast<unsigned char>(stoi(recordArr[i][7]));
            cout<< r.rebHome << endl;
        }
        catch(const exception& e)
        {
            r.rebHome = static_cast<unsigned char>(0);
            missing += pow(2,0);
        }
        cout<< "REB: " << recordArr[i][7] << " --> " << r.rebHome << endl;
        
        r.homeTeamWins= (bool)(stoi(recordArr[i][8]));
        cout<< "win: " << recordArr[i][8] << " --> " << r.homeTeamWins << endl;

        r.recorderHeader = missing;
        
        cout<<"Empty fields: "<<r.recorderHeader<<endl;
        cout<< "bytesize = "<< sizeof(r) << endl;
    
        // recordBytes.push_back(r);
        }
         

};

//check for the conversion 
//have a reverse conversion