#ifndef VIEW_H
#define VIEW_H
#include "MenuStrings.h"
#include "MEMORY.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

//Global constants
enum resolution :size_t{WIDTH = 120, HEIGHT = 30, PRINT_LINE = 4}; //Resolution line consts
enum errorTypes :size_t{SIGN = 1, INV_OPCODE = 2, OVER_FLOW = 3, UNDER_FLOW = 4};//Error types thrown by CPU
enum mainPages :size_t{MAIN = 0, README_1 = 1, README_2 = 2, README_3 = 3, README_4 = 4, README_5 = 5,
    EDIT = 6, EXEC = 7, SAVE = 8, LOAD = 9, DUMP = 10};
const char LINE_TOKEN[] = "%l", PAGE_TOKEN[] = "%p", ADDR_TOKEN[] = "%a", VAL_TOKEN[] = "%v",
            DUMP_TOKEN[] = "%d", STATUS_TOKEN[] = "%s"; // String token consts

class VIEW{
public:
    //Constructor
    VIEW(){currPageNum = 0;};

//Displays a requested page
    void Display(const int& p){
currPageNum = p; //At page 'p'
        currPage.str(menus.getMenu(p)); //Get menu page from library facade

        //Check if loaded page is right resolution of 120x30 chars
        Validate(stringstream(currPage.str()));

        //Check which page was loaded to put fist page num
        if(p == EDIT || p == LOAD || p == EXEC)
            currPage.str(regex_replace(currPage.str(), regex(PAGE_TOKEN), to_string(1)+" "));

        else if (p == MAIN)
            currPage.str(regex_replace(currPage.str(), regex(STATUS_TOKEN), "  "));

        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Update View
        cout.flush();
    }

    //Display status of load or save. true = load, false = save
    void MainError(size_t menu, bool status){
        //Load up main menu
        currPageNum = MAIN;
        currPage.str(menus.getMenu(MAIN));

        //Switch case for menu
        if (menu == 1){ //Load
            currPage.str(regex_replace(currPage.str(), regex(string(string(STATUS_TOKEN) + "( ){" +
                                           string(to_string((status)? (LOAD_SUCCESS.length()-2):(LOAD_FAIL.length()-2)) + "}"))),
                                       (status)? LOAD_SUCCESS : LOAD_FAIL));
        }
        else if(menu == 2){ //Save
            currPage.str(regex_replace(currPage.str(), regex(string(string(STATUS_TOKEN) + "( ){" +
                                        string(to_string((status)? (SAVE_SUCCESS.length()-2):(SAVE_FAIL.length()-2)) + "}"))),
                                       (status)? SAVE_SUCCESS : SAVE_FAIL));
        }
        else{ //Execution mode rejected
            currPage.str(regex_replace(currPage.str(), regex(string(string(STATUS_TOKEN) + "( ){" +
                                        string(EXECUTION_REJECTION) + "}")), EXECUTION_REJECTION));
        }

        //Check if loaded page is right resolution of 120x30 chars
        Validate(stringstream(currPage.str()));

        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Update View
        cout.flush();
    }

    //Display line on screen in Edit Mode
    void DispLine(const string& line){
        //Check if an error is on the page
        if (currPage.str().find(INV_MESSAGE) > 0) { //Just reprint page, without error
            cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Update View
            cout.flush();
            return;
        }

        if (currPageNum != EDIT)//Make sure we are in the right page.
            throw logic_error(string("ERROR: Simulator attempted to display a line in Edit mode while in page"
                                           + to_string(currPageNum)));

        //If max line has been reached, add new page
        lineSpace++;
        if (lineSpace >= HEIGHT){
            execPages.push_back(currPage.str()); //Push to page vector
            currPage.str(menus.getMenu(EDIT));
            currPage.str(regex_replace(currPage.str(), regex(PAGE_TOKEN),
                                       string(execPages.size()+((execPages.size()<10)? " ":""))));
            lineSpace = PRINT_LINE+1;
        }

        currPage.str(regex_replace(currPage.str(), regex(string(LINE_TOKEN)+"   "), line)); //Update line

        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Update View
        cout.flush();
    }

    //Continues the memory state of Edit Mode
    void ContinueEdit(MEMORY mem){
        if (currPageNum != MAIN)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to access Edit Mode not from the main menu, "
                                           "but instead in page " + to_string(currPageNum)));

        currPage.str(menus.getMenu(EDIT)); //Load up the Edit Page
        size_t page =  mem.getMap().size()/13; //What page?
        currPage.str(regex_replace(currPage.str(), regex(PAGE_TOKEN),
                                   string(page+((page<10)? " ":"")))); //Put page no

        for(size_t i = mem.getMap().size() % page; i < mem.getMap().size(); i++){ // Replace line tokens with memory
            currPage.str(regex_replace(currPage.str(), regex(string(LINE_TOKEN)+"   "), mem.getMap().at(i)));
        }
        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Update View
        cout.flush();
    }

    //User's input is invalid, print message on view
    void DisplayInvalid(){
        //Check if an error is already on the page
        if (currPage.str().find(INV_MESSAGE) > 0) { //Just reprint page, same error
            cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Update View
            cout.flush();
            return;
        }

//        else //No error currently on screen, just print
//            currPage << INV_MESSAGE;

        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Re-display View
        cout << INV_MESSAGE;// Display message
        cout.flush();
    }

    //Error encountered during execution mode, print message on view
    //type, line, opcode
    void DisplayError(size_t type, size_t line, size_t opcode){
        if (currPageNum != EXEC)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to display an Execution Error while in page "
                                           + to_string(currPageNum) + " instead of Execution Page"));

        //If max line has been reached, add new page
        lineSpace++;
        if (lineSpace >= HEIGHT){
            execPages.push_back(currPage.str()); //Push to page vector
            currPage.str(menus.getMenu(EDIT));
            currPage.str(regex_replace(currPage.str(), regex(PAGE_TOKEN),
                                       string(execPages.size()+((execPages.size()<10)? " ":""))));
            lineSpace = PRINT_LINE+1;
        }

        string error;

        //Error type switch case
        switch (type) {
            case SIGN:
                currPage.str(regex_replace(currPage.str(), regex(string(LINE_TOKEN)+
                                            "( ){"+to_string(SIGN_ERROR.size()-2)+"}"), SIGN_ERROR));
                currPage.str(regex_replace(currPage.str(), regex(ADDR_TOKEN), to_string(line)+((line<10)?" ":"")));
                break;
            case INV_OPCODE:
                currPage.str(regex_replace(currPage.str(), regex(string(LINE_TOKEN)+
                                            "( ){"+to_string(INV_ERROR.size()-2)+"}"), INV_ERROR));
                currPage.str(regex_replace(currPage.str(), regex(VAL_TOKEN), to_string(opcode)));
                currPage.str(regex_replace(currPage.str(), regex(ADDR_TOKEN), to_string(line)+((line<10)?" ":"")));
                break;
            case OVER_FLOW:
                currPage.str(regex_replace(currPage.str(), regex(string(LINE_TOKEN)+
                                            "( ){"+to_string(OVER_WARNING.size()-2)+"}"), OVER_WARNING));
                break;
            case UNDER_FLOW:
                currPage.str(regex_replace(currPage.str(), regex(string(LINE_TOKEN)+
                                            "( ){"+to_string(UNDER_WARNING.size()-2)+"}"), UNDER_WARNING));
                break;
            default:
                throw(logic_error(string("CPU threw an unknown error of type " + to_string(type) + "... EXITING SIMULATOR")));
        }

        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), ""); //Update View
        cout.flush();
    }

    //Display the write message on the Exec page
    void DisplayWrite(const int& addr, const int& value){
        if (currPageNum != EXEC)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to display the Write opcode message while in page "
                                            + to_string(currPageNum) + " instead of Execution Page"));

        //Make message template
        string output = WRITE_TEMPLATE;
        output = regex_replace(output, regex(ADDR_TOKEN), to_string(addr));
        output = regex_replace(output, regex(VAL_TOKEN), to_string(value));

        //If max line has been reached, add new page
        lineSpace++;
        if (lineSpace >= HEIGHT){
            execPages.push_back(currPage.str()); //Push to page vector
            currPage.str(menus.getMenu(EXEC));
            currPage.str(regex_replace(currPage.str(), regex(PAGE_TOKEN),
                                       to_string(execPages.size())+((execPages.size()<10)?" ":"")));
            lineSpace = PRINT_LINE+1;
        }

        currPage.str(regex_replace(currPage.str(), regex(string(LINE_TOKEN)+"( ){"+
                                        to_string(output.size()-2)+"}"), output)); //Put write message in page
        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Update View
        cout.flush();
    }

    //Display message that Halt was reached in execution mode
    void DisplayEnd(){
        if (currPageNum != MAIN)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to display the HALT opcode message while in page "
                                       + to_string(currPageNum) + " instead of Execution Page"));

        //If max line has been reached, add new page
        lineSpace++;
        if (lineSpace >= HEIGHT){
            execPages.push_back(currPage.str()); //Push to page vector
            currPage.str(menus.getMenu(EXEC));
            currPage.str(regex_replace(currPage.str(), regex(PAGE_TOKEN), to_string(execPages.size())+
                                            ((execPages.size()<10)?" ":"")));
            lineSpace = PRINT_LINE+1;
        }

        currPage.str(regex_replace(currPage.str(), regex(string(LINE_TOKEN)+"( ){"+
                                        to_string(HALT_MESSAGE.size()-2)+"}"), HALT_MESSAGE)); //Put read message in page

        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Update View
        cout.flush();
    }

    //Display the read message on the Exec page
    void DisplayRead(){
        if (currPageNum != EXEC)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to display the Read opcode message while in page "
                                       + to_string(currPageNum) + " instead of Execution Page"));

        //If max line has been reached, add new page
        lineSpace++;
        if (lineSpace >= HEIGHT){
            execPages.push_back(currPage.str()); //Push to page vector
            currPage.str(menus.getMenu(EXEC));
            currPage.str(regex_replace(currPage.str(), regex(PAGE_TOKEN), to_string(execPages.size())+
                                            ((execPages.size())?" ":"")));
            lineSpace = PRINT_LINE+1;
        }

        currPage.str(regex_replace(currPage.str(), regex(string(LINE_TOKEN)+"( ){"+
                                        to_string(READ_TEMPLATE.size())+"}"), READ_TEMPLATE)); //Put read message in page
        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), "  "); //Update View
        cout.flush();
    }

    //End of execution, display dump and reset exec variables
    /// CHECK WITH TEAM FOR IMPLEMENTATION
//    void DisplayDump(const string& dump){
//        lineSpace = PRINT_LINE; //Reset line counter
//        execPages.clear(); //Clear execution page history.
//
//        stringstream dumpPage(menus.getMenu(71));
//        dumpPage.str(regex_replace(dumpPage.str(), regex(DUMP_TOKEN), dump));
//
//        currPage.str(dumpPage.str()); //Is now current page
//        Validate(stringstream(currPage.str())); //Check if page is valid
//
//        cout << regex_replace(currPage.str(), regex(LINE_TOKEN), ""); //Update View
//        cout.flush();
//    }

private:
    MenuStrings menus;
    stringstream currPage; //What is currently displayed on the view
    size_t currPageNum; //What page are we in right now?
    vector<string> execPages; //Page history in Execution mode
    int lineSpace = PRINT_LINE; //Keep track of line space in

    //CONST string templates
    const string INV_MESSAGE = "ERROR: Invalid Option, try again. >";
    const string LOAD_SUCCESS = "Memory Successfully Loaded!";
    const string LOAD_FAIL = "Memory Could Not Load";
    const string SAVE_SUCCESS = "Memory Successfully Saved!";
    const string SAVE_FAIL = "Memory Could Not Save";
    const string EXECUTION_REJECTION = "No memory has been staged yet";
    const string SIGN_ERROR = "Error: Invalid word sign detected at line %a... Ending Execution";
    const string INV_ERROR = "Error: Invalid opcode %v read at address %a... Ending Execution";
    const string OVER_WARNING = "Warning: Overflow detected";
    const string UNDER_WARNING = "Warning: Underflow detected";
    const string WRITE_TEMPLATE = "Memory location %a currently has %v"; //Contains Regex tokens
    const string READ_TEMPLATE = "Please input a number: ";
    const string HALT_MESSAGE = "HALT Reached. Press any key to display memory dump.";


    //Check if the currPage stream is valid for the console
    void Validate(stringstream check){
//        string page = currPage.str();
        string lines[29];

        //Grab strings
        for(std::size_t i = 0; i < HEIGHT; i++)
            if (check.rdbuf()->in_avail() != 0) {
                getline(check, lines[i]);
                if (lines[i].length() > WIDTH) //Check width
                    throw logic_error(string("Page output is incorrect width resolution at line ", i));
            }

        //Check if there are still lines in the stream
        if (check.rdbuf()->in_avail() != 0)
            throw logic_error("Page output is incorrect height resolution");

        //Validation passed
    }
};

#endif