#ifndef TEST_DRIVER_H
#define TEST_DRIVER_H
#include <iostream>
#include <sstream>
#include <thread>
#include <string>
#include <fstream>
#include "test.h"
#include "VIEW.h"
#include "CONTROLLER.h"
#include "MODEL.h"
#include "CPU.h"

enum MenuOptions :size_t{README_MEN = 1, EDIT_MEN = 2, LOAD_MEN = 3, SAVE_MEN = 4, EXEC_MEN = 5, EXIT_MEN = 6};
VIEW view;

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

	CPU
        -Execute a halt program and check if results are accurate
        -Execute an addition program and check if results are accurate
            9000+1000 = 0001_0000
		 Check if execution is sending correct errors and warnings to VIEW
		    -Overflow warning
		    -Invalid opcode

 */

void testing(){
    MODEL model((VIEW&) view);
    CONTROLLER controller((MODEL&)model, (VIEW&)view);
};

int main(){

    const string saveFile = "Save.mem";


    //Change cin buffer for testing
    std::streambuf *CIN_OLD;
    CIN_OLD = std::cin.rdbuf();
    stringstream input;
    std::cin.rdbuf(input.rdbuf());

    //Init Simulator thread
    std::thread sim (testing);

    ///                    Testing Start
    ///                     CONTROLLER
    ///------------------------------------------------------------


    ///Execute StartSimulator and test if at main menu
    test_(view.currMenu == MAIN);

    //Controller attempts to update view at appropriate times
    //Go to readme
    input << to_string(README_MEN) << std::endl;
    test_(view.currMenu == README_1); ///Requests readme
    input << "1" << std::endl; //Exit Readme

    //Go to edit
    input << to_string(EDIT_MEN) << std::endl;
    test_(view.currMenu == EDIT); ///Requests Edit
    input << "-99999" << std::endl;

    //Check if Controller is sending errors to view in...
    //Go to load
    input << to_string(LOAD_MEN) << std::endl;
    test_(view.currMenu == MAIN
            && view.s_Display == to_string(LOAD)+" false"); ///Load Deny
    input << "2" << std::endl;

    //Go to save
    input << to_string(SAVE_MEN) << std::endl;
    test_(view.currMenu == MAIN
            && view.s_Display == to_string(SAVE)+" false"); ///Save Deny

    //Go to execute
    input << to_string(EXEC_MEN) << std::endl;
    test_(view.currMenu == MAIN
            && view.s_Display == to_string(EXEC)+" false"); ///Execute Deny

    //Check if Controller loads and saves correctly
    //Go to edit and create one line +4300, exit with +9999, and test hasMemory() by going to save menu
    input << EDIT_MEN << std::endl;
    input << "+4300" << std::endl;
    input << "-99999" << std::endl;
    input << SAVE_MEN << std::endl;

    test_(view.currMenu == SAVE); ///Save Allowed

    //Check displayed save contents
    test_(view.s_Display == "+4300"); ///Save Display
    input << 1 << std::endl;

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
    input << LOAD_MEN << std::endl;
    test_(view.s_Display == "+4300"); ///Load display
    input << 3 << std::endl;
    test_(view.s_Display == "EMTPY"); ///Correct message when erased memory
    input << EDIT_MEN << std::endl;
    input << "+0000" << std::endl;
    input << "-99999" << std::endl;
    input << LOAD_MEN << std::endl;
    input << 1 << std::endl;
    test_(view.s_Display == (to_string(LOAD) + " true")); ///Load message
    input << EDIT_MEN << std::endl;
    test_(view.s_Display == "+4300"); ///Load was correct
    input << "-99999" << std::endl;


    ///                         CPU
    ///------------------------------------------------------------


    //Execute, and test if result passed
    input << EXEC_MEN << std::endl;
    input << std::endl << std::endl;

    test_((view.s_Display == "END"
            && n_accumulator == 0
            && n_opcode == 43
            && n_operand == 0
            && n_IC == 0
            && n_IR == "+4300")); ///Halt program executed correctly

    input << std::endl << std::endl;

    //Go to load, erase staged memory, then go to edit
    input << LOAD_MEN << std::endl;
    input << 3<< std::endl;
    input << EDIT_MEN << std::endl;

    //Input addition program and save
    input << "+1007" << std::endl; //0
    input << "+1008" << std::endl; //1
    input << "+2007" << std::endl; //2
    input << "+3008" << std::endl; //3
    input << "+2109" << std::endl; //4
    input << "+1109" << std::endl; //5
    input << "+4300" << std::endl; //6
    input << "-99999" << std::endl;

    //Go to execute and compare dump
    input << EXEC_MEN << std::endl;
    input << 9000 << std::endl;
    input << 1000 << std::endl;
    input << std::endl;

    //CHECK WITH ANTHONY ON IMPLEMENTATION
    test_(n_result[7] == "+9000"
            && n_result[8] == "++1000"
            && n_result[9] == "++0001"
            && n_result[10] == "+0000"); ///Addition Program executed correctly

    //Repeat creating an addition program but put in an warnings and end in an error
    input << std::endl;
    input << LOAD_MEN << std::endl;
    input << 3 << std::endl;
    input << EDIT_MEN << std::endl;

    input << "+1007" << std::endl; //0
    input << "+1008" << std::endl; //1
    input << "+2007" << std::endl; //2
    input << "+3308" << std::endl; //3
    input << "+2109" << std::endl; //4
    input << "+1109" << std::endl; //5
    input << "+1000" << std::endl; //6
    input << "-99999" << std::endl;

    //Execute, check if errors and warnings were attempted to be displayed
    input << EXEC_MEN << std::endl;
    input << 1000 << std::endl;
    input << 1000 << std::endl;
    test_(view.s_Display == "3 0 0"); ///Overflow
    input << 0 << std::endl;
    test_(view.s_Display == "2 7 0"); ///Invalid opcode

    //Exit simulator, testing done
    input << std::endl;
    input << 6 << endl;

    sim.join();
    results_();

    if (remove(saveFile.c_str()) != 0)
        cout << "SAVE FILE COULD NOT BE ERASED, ERASE Save.mem BEFORE NEXT TEST"
             << std::endl;

    std::cin.rdbuf(CIN_OLD); //Return cin buffer to normal
    cin.get();
    return 0;
}
#endif