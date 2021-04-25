#ifndef TEST_VIEW_H
#define TEST_VIEW_H
#include "test.h"
//#include "CPU.h"
//#include "MODEL.h"
#include "MEMORY.h"
#include <string>
#include <vector>
#include <utility>
#include <map>
using namespace std;

enum messageType :size_t    {INV_MESSAGE = 0, LOAD_SUCCESS = 1, LOAD_FAIL = 2, SAVE_SUCCESS = 3, SAVE_FAIL = 4,
    LOAD_REJECTION = 5, EXECUTION_REJECTION = 6, SIGN_ERROR = 7, INV_ERROR = 8,
    OVER_WARNING = 9, UNDER_WARNING = 10, WRITE_TEMPLATE = 11, READ_TEMPLATE = 12,
    HALT_MESSAGE = 13, SUB_REJ_MESSAGE = 14, SUB_CLIP_MESSAGE = 15};

enum mainPages :size_t      {MAIN = 0, README_1 = 1, README_2 = 2, README_3 = 3, README_4 = 4, README_5 = 5, README_6 = 6,
    EDIT = 7, EXEC = 8, SAVE = 9, LOAD = 10};

enum subPages :size_t       {ED_COPY = 11, ED_CUT = 12, ED_PASTE = 13, ED_INSERT = 14, ED_DELETE = 15, NEXT_SUB = 16,
    PREV_SUB = 17};


map<size_t, string> n_result;
int n_accumulator, n_IC, n_operand;
string n_IR;
size_t n_opcode;


//This is a test dummy view. All will need to be visible for driver
struct VIEW{
    //Variables
    size_t currMenu;
    size_t testingMode;
    string s_Display;
    string& clipboard;

    VIEW(string& c): clipboard(c) {currMenu = 0; s_Display = ""; testingMode = 0;}

    //Resets data for next test
    void Reset(){
        currMenu = 0;
        s_Display = "";

        n_result.clear();
        n_accumulator = 0;
        n_IC = 0;
        n_operand = 0;
        n_IR = "";
        n_opcode = 0;
    }

    //Update menu number
    void Display(const int& p){
        if (testingMode == 1 && p != MAIN){
            currMenu = p;
        }

        else if (testingMode != 1){
            currMenu = p;

            if (p == EDIT)
                s_Display = "EMPTY";
        }
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
        if (testingMode != 1){
            currMenu = MAIN;
            s_Display = string(to_string(menu) + " " + ((status) ? "true" : "false"));
        }
    }

    //Update what is going on the screen and menu number
    void DisplayInvalid(size_t type){
        s_Display = "INVALID";
    }
    void DisplayError(size_t type, size_t line, size_t opcode){
        s_Display = string(to_string(type) + " " + to_string(line) + " " + to_string(opcode));
    }
    void DispLine(const string& line){
        s_Display = line;
    }
    void DisplayEnd(){
        if (testingMode != 4){ s_Display = "END"; }
    }
    void DisplayWrite(const int& addr, const int& value){
        s_Display = string(to_string(addr) + " " + to_string(value));
    }
    void DisplayRead(string value){
        s_Display = move(value);
    }
};


#endif