#ifndef TEST_DRIVER_H
#define TEST_DRIVER_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <string>
#include "test.h"
#include "VIEW.h"
#include "CONTROLLER.h"

using namespace std::chrono_literals;

enum MenuOptions :size_t{README_MEN = 1, EDIT_MEN = 2, LOAD_MEN = 3, SAVE_MEN = 4, EXEC_MEN = 5, EXIT_MEN = 6};
enum SubMenus :size_t{COPY_MEN = 1, CUT_MEN = 2, PASTE_MEN = 3, INSERT_MEN = 4, DELETE_MEN = 5};

std::streambuf *CIN_OLD = std::cin.rdbuf();

string clipboard;
VIEW view(clipboard);
static stringstream input;
std::ifstream test;
//std::ofstream in;


/// Unit Tests written by Daniel Espinel and Dane Manley
/*
                            TESTING METHODOLOGY
  -------------------------------------------------------------------------
  Due to simulator relying on cin for input, cin buffer will be replaced with
  a controlled ostream. VIEW.h and MemoryDump.h will be the repurposed for
  relaying information to the test diver, thus, they are dummy repurposed
  classes.

  * DO NOT REPLACE VIEW.h OR MemoryDump.h WITH THE REAL VERSIONS! IF YOU DO
  * BY ACCIDENT, RE-DOWNLOAD FROM DEV OR danBranch OR ASK ME (Daniel) ON
  * DISCORD

  TESTED CLASSES
  -CONTROLLER
  -CPU

  TESTS
  	CONTROLLER
		-Execute StartSimulator and test if at main menu
		 Controller attempts to update view at appropriate times
		    -Requests readme fine
            -Requests Edit fine
		     Error when entering without memory staged or save file present...
		        -Load
		        -Save
		        -Execute
		 Check if Controller loads and saves correctly
			-Save menu is accessed after memory is staged
			-Correct data is displayed in save menu
			-Correct message is displayed when save is successful
			-File has correct data saved
			-Correct message is displayed when staged memory is erased
			-Correctly displays message when load is successful
			-Load was successful
       Check if edit mode sub-menus operate correctly
            -Cut line
            -Copy and Insert line
            -Delete line
            -Paste line

	CPU
        -Execute a halt program and check if results are accurate
        -Execute an addition program and check if results are accurate
            9000+1000 = 0001_0000
		 Check if execution is sending correct error to VIEW
		    -Invalid opcode

 */

void testing(){
    view.Reset();

    MODEL model((VIEW&) view);
    CONTROLLER controller((MODEL&)model, (VIEW&)view, (string&) clipboard);
    controller.StartSimulator();

    input.str("");
//    std::cin.rdbuf(CIN_OLD); //Return cin buffer to normal
};

void type(string str){
    input << str << std::endl;
}

int main(){
    const string saveFile = "Save.mem";


    //Change cin buffer for testing
    std::cin.rdbuf(input.rdbuf());

    //Init Simulator thread
//    std::thread sim (testing);

    ///                    Testing Start
    ///                     CONTROLLER
    ///------------------------------------------------------------


    ///Execute StartSimulator and test if at main menu
    type("6");
    testing();
//    std::this_thread::sleep_for(1000ms); //Sleep to not run into thread fighting
    test_(view.currMenu == MAIN);

    //Controller attempts to update view at appropriate times
    view.testingMode = 1;
    //Go to readme
    type(to_string(README_MEN));
    type("1");
    type("6");
    testing();
    test_(view.currMenu == README_1); ///Requests readme

    //Go to edit
    type(to_string(EDIT_MEN));
    type("-99999");
    type("6");
    testing();
    test_(view.currMenu == EDIT); ///Requests Edit

    //Check if Controller is sending errors to view in...
    view.testingMode = 2;
    //Go to load
    type(to_string(LOAD_MEN));
    type("6");
    testing();
    test_(view.currMenu == MAIN && view.s_Display == to_string(LOAD_MEN)+" false"); ///Load Deny

    //Go to save
    type(to_string(SAVE_MEN));
    type("6");
    testing();
    test_(view.currMenu == MAIN
            && view.s_Display == to_string(SAVE_MEN)+" false"); ///Save Deny

    //Go to execute
    type(to_string(EXEC_MEN));
    type("6");
    testing();
    test_(view.currMenu == MAIN
            && view.s_Display == to_string(4)+" false"); ///Execute Deny

    //Check if Controller loads and saves correctly
    //Go to edit and create one line +4300, exit with +99999, and test hasMemory() by going to save menu
    view.testingMode = 1;
    type(to_string(EDIT_MEN));
    type("+4300");
    type("-99999");
    type(to_string(SAVE_MEN));
    type("1");
    type("6");
    testing();
    test_(view.currMenu == SAVE); ///Save Allowed

    //Check displayed save contents
    test_(view.s_Display == "+4300"); ///Save Display

    //Save file has correct information
    string saveTest;
    ifstream save(saveFile, ios::in);
    if (save.is_open()) {
        save >> saveTest;
        test_(saveTest == "+4300"); ///File Correctly Saved
    }
    else
        test_(save.is_open()); ///FILE COULD NOT OPEN
    save.close();

    //Change staged memory to check load
    view.testingMode = 2;
    type(to_string(LOAD_MEN));
    type("2");
    type("6");
    testing();
    test_(view.s_Display == "+4300"); ///Load display

    type(to_string(LOAD_MEN));
    type("1");
    type(to_string(LOAD_MEN));
    type("3");
    type(to_string(EDIT_MEN));
    type("-99999");
    type("6");
    testing();
    test_(view.s_Display == "EMPTY"); ///Correct message when erased memory

    type(to_string(EDIT_MEN));
    type("+0000");
    type("-99999");
    type(to_string(LOAD_MEN));
    type("1");
    type("6");
    testing();
    test_(view.s_Display == (to_string(1) + " true")); ///Load message

    type(to_string(EDIT_MEN));
    type("+0000");
    type("-99999");
    type(to_string(LOAD_MEN));
    type("1");
    type(to_string(EDIT_MEN));
    type("-99999");
    type("6");
    testing();
    test_(view.s_Display == "+4300"); ///Load was correct

    //Check if edit mode sub-menus operate correctly
    type(to_string(EDIT_MEN));
    type("+0000");
    type("+0001");

    //Cut last line
    type(to_string(CUT_MEN));
    type("01");
    type("-99999");
    type("6");
    testing();
    test_(view.clipboard == "+0001" && view.s_Display == "+0000"); /// Test Cut (Line is Only in Clipboard)

    //Insert to line before last
    type(to_string(EDIT_MEN));
    type("+0000");
    type("+0001");
    type(to_string(COPY_MEN));
    type("01");
    type(to_string(INSERT_MEN));
    type("00");
    type("-99999");
    type("6");
    testing();
    test_(view.s_Display == "+0001"); ///Test Copy and Insert (Shifted Lines)

    //Delete last line
    type(to_string(EDIT_MEN));
    type("+0000");
    type("+0001");
    type(to_string(DELETE_MEN));
    type("00");
    type("-99999");
    type("6");
    testing();
    test_(view.s_Display == "+0001"); ///Test Delete

    //Paste to last line
    type(to_string(EDIT_MEN));
    type("+0000");
    type("+0001");
    type("+0000");
    type(to_string(COPY_MEN));
    type("01");
    type(to_string(PASTE_MEN));
    type("00");
    type("-99999");
    type("6");
    testing();
    test_(view.s_Display == "+0001"); ///Test Paste (Overwrite Line)


    ///                         CPU
    ///------------------------------------------------------------


    //Execute, and test if result passed
    type(to_string(LOAD_MEN));
    type("1");
    type(to_string(EXEC_MEN));
    type("");
    type("");
    type("6");
    testing();

    test_((view.s_Display == "END"
            && n_accumulator == 0
            && n_opcode == 43
            && n_operand == 0
            && n_IC == 0
            && n_IR == "+4300")); ///Halt program executed correctly

    /// Dane Manley Begin

    //Create addition program and save
    type(to_string(EDIT_MEN));
    type("+1007"); //0
    type("+1008"); //1
    type("+2007"); //2
    type("+3008"); //3
    type("+2109"); //4
    type("+1109"); //5
    type("+4300"); //6
    type("-99999");

    //Go to execute and compare dump
    type(to_string(EXEC_MEN));
    type("9000");
    type("1000");
    type("");
    type("");
    type("6");
    testing();

    //CHECK WITH ANTHONY ON IMPLEMENTATION
    test_(n_result[7] == "+9000"
            && n_result[8] == "+1000"
            && n_result[9][1] == '+'
            && n_result[10][1] == '+'); ///Addition Program executed correctly

    //Repeat creating an addition program but end in an error
    type(to_string(EDIT_MEN));
    type("+1007"); //0
    type("+1008"); //1
    type("+2007"); //2
    type("+3308"); //3
    type("+2109"); //4
    type("+1109"); //5
    type("+0000"); //6
    type("-99999");

    //Execute, check if errors and warnings were attempted to be displayed
    view.testingMode = 4;
    type(to_string(EXEC_MEN));
    type("1");
    type("2");
    type("");
    type("");
    type("6");
    testing();
    test_(view.s_Display == "2 6 0"); ///Invalid opcode

    /// Dane Manley End

    results_();


    if (remove(saveFile.c_str()) != 0)
        cout << "SAVE FILE COULD NOT BE ERASED, ERASE Save.mem BEFORE NEXT TEST"
             << std::endl;

    std::cin.rdbuf(CIN_OLD); //Return cin buffer to normal
    cin.get();
    return 0;
}
#endif