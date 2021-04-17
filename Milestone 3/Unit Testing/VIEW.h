#ifndef TEST_VIEW_H
#define TEST_VIEW_H
#include "test.h"
//#include "CPU.h"
//#include "MODEL.h"
#include "MEMORY.h"
#include <string>
#include <vector>
#include <utility>
using namespace std;

enum mainPages :size_t{MAIN = 0, README_1 = 1, README_2 = 2, README_3 = 3, README_4 = 4, README_5 = 5,
    EDIT = 6, EXEC = 7, SAVE = 8, LOAD = 9};
map<size_t, string> n_result;
int n_accumulator;
int n_IC;
int n_operand;
string n_IR;
size_t n_opcode;


//This is a test dummy view. All will need to be visible for driver
struct VIEW{
    //Variables
    size_t currMenu;
    string s_Display;


    VIEW() {currMenu = 0; s_Display = "";}

    //Update menu number
    void Display(const int& p){
        currMenu = p;
        s_Display = "";

        n_result.clear();
        n_accumulator = 0;
        n_IC = 0;
        n_operand = 0;
        n_IR = "";
        n_opcode = 0;

        if (p == EDIT)
            s_Display = "EMPTY";
    }

    //Update what is on the screen
    void DisplayLoadSave(bool menu, MEMORY load){
        currMenu = (menu)?LOAD:SAVE;
        s_Display = load.getMap()[0];
    }
    void ContinueEdit(MEMORY mem){
        currMenu = EDIT;
        s_Display = mem.getMap()[0];
    }
    void MainError(size_t menu, bool status){
        currMenu = MAIN;
        s_Display = string(to_string(menu) + " " + ((status)?"true":"false"));
    }

    //Update what is going on the screen and menu number
    void DisplayInvalid(){
        s_Display = "INVALID";
    }
    void DisplayError(size_t type, size_t line, size_t opcode){
        s_Display = string(to_string(type) + " " + to_string(line) + " " + to_string(opcode));
    }
    void DispLine(const string& line){
        s_Display = line;
    }
    void DisplayEnd(){
        s_Display = "END";
    }
    void DisplayWrite(const int& addr, const int& value){
        s_Display = string(to_string(addr) + " " + to_string(value));
    }
    void DisplayRead(string value){
        s_Display = move(value);
    }
};


#endif