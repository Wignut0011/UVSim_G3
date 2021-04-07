#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include "CPU.h"
#include <fstream>
using namespace std;

//For testing purposes, will return a test map
//1 = Arithmetic test (input 2 numbers)
//2 = Branch test
//3 = Cond Branch test (result from adding 2 numbers)
map<size_t, string> TestMaps(int opt){
    map<size_t, string> testMap;
    switch (opt) {
        //arithmetic test
        case 1:
            testMap = {{0, "+1007"}, {1, "+1008"}, //03 is arithmetic instruction
                       {2, "+2007"}, {3, "+3308"},
                       {4, "+2109"}, {5, "+1109"},
                       {6, "+4300"}, {7, "+0000"},
                       {8, "+0000"}, {9, "+0000"}};
            for (int i = 10; i < 100; ++i)
                testMap[i] = "+0000";
            break;

            //branch test
        case 2:
            testMap = {{0, "+4004"}, {1, "+4300"}, //Prints 1 if successful
                       {2, "+0001"}, {3, "+0000"},
                       {4, "+1102"}, {5, "+4300"},
                       {6, "+0000"}, {7, "+0000"},
                       {8, "+0000"}, {9, "+0000"}};
            for (int i = 10; i < 100; ++i)
                testMap[i] = "+0000";
            break;

            //conditional branch test
        case 3:
            testMap = {{0, "+1007"}, {1, "+1008"}, //If add is neg, print 1
                       {2, "+2007"}, {3, "+3008"},  //If add is 0, print 2
                       {4, "+2109"}, {5, "+4110"},
                       {6, "+4213"}, {7, "+0000"},
                       {8, "+0000"}, {9, "+0000"},
                       {10,"+1112"}, {11,"+4300"},
                       {12,"+0001"}, {13,"+1115"},
                       {14,"+4300"}, {15,"+0002"}};
            for (int i = 16; i < 100; ++i)
                testMap[i] = "+0000";
            break;
    }
    return testMap;
}

//Prints memory dump
void MemDump(map<size_t, string> inputMap){
    cout << "MEMORY\n       00     01     02     03     04     05     06     07     08     09" << endl;
    string line;
    for (int i = 0; i < 100; i++){
        if (i % 10 == 0){
            if (i == 0)
                line = "00";

            else
                line = to_string(i);
        }
        string memory = inputMap[i];
        int addZero =  5 - memory.length();

        for(int j = 0; j < addZero; j++)
            memory.insert(1,"0");

        line += "  " + memory;
        if (i % 10 == 9)
            cout << line << endl;
    }
}

int main () {
//post welcome message and instructions
    cout << "**** Welcome to UV SIM! ****\n" <<
         "**** Please enter your program one instruction ****\n" <<
         "**** (or data word) at a time into the input   ****\n" <<
         "**** text field. I will display the location   ****\n" <<
         "**** and a question mark (?). You then         ****\n" <<
         "**** type the word for that location. Enter    ****\n" <<
         "**** -99999 to stop entering your program      ****" << endl;
    //initialize map
    map<size_t, string> inputMap;
    bool done = false;



    for (size_t i = 0; i < 100; i++) {
        if (!done) {
            string line;
            if (i < 10) line = "0" + to_string(i);
            else line = to_string(i);
            cout << line << " ? "; cout.flush();

            //gather user input
            string uInput;
            cin >> uInput;

            //stop gathering if user types -99999
            if (uInput == "-99999"){
                done = true;
                uInput = "+0000";
            }

            //place user input into map
            inputMap[i] = uInput;
        }

            // Format rest of empty cells
        else
            inputMap[i] = "+0000";
    }
    //Print message to user that the program is now executing
    cout << "------------Exiting Edit Mode------------\n---------Entering Execution Mode---------\n" << endl;



    //Run program
    CPU exe(inputMap);

    //call CPU and send inputMap to it.
    MemDump(inputMap);

    cout << "\nPress any key to exit..."; cout.flush();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear cin stream
    cin.get();

    ofstream file ("test.txt");
    if (file.is_open())
    {
        for(int i = 0; i < inputMap.size(); i++)
        {
            file << inputMap[i] << endl;
        }
        file.close();
    }
    else cout << "error with file\n";



    return 0;
}