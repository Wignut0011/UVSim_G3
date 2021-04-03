#ifndef VIEW_H
#define VIEW_H
#include "MenuStrings.h"
#include "MEMORY.h"
//#include "CPU.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

//Enumerations, these are not attributes.
enum resolution :size_t{WIDTH = 120, HEIGHT = 30, PRINT_LINE = 19}; //Resolution line consts
enum errorTypes :size_t{SIGN = 1, INV_OPCODE = 2, OVER_FLOW = 3, UNDER_FLOW = 4};//Error types thrown by CPU
enum messageType :size_t{INV_MESSAGE = 0, LOAD_SUCCESS = 1, LOAD_FAIL = 2, SAVE_SUCCESS = 3, SAVE_FAIL = 4, 
                        LOAD_REJECTION = 5, EXECUTION_REJECTION = 6, SIGN_ERROR = 7, INV_ERROR = 8, OVER_WARNING = 9, 
                        UNDER_WARNING = 10, WRITE_TEMPLATE = 11, READ_TEMPLATE = 12, HALT_MESSAGE = 13};
enum mainPages :size_t{MAIN = 0, README_1 = 1, README_2 = 2, README_3 = 3, README_4 = 4, README_5 = 5,
                        EDIT = 6, EXEC = 7, SAVE = 8, LOAD = 9};
enum formatting :size_t{CURR_PAGE_NUM = 0, EXEC_PAGES = 1, MEM_PAGES = 2, LINE_SPACE = 3};
enum tokenIndex :size_t{LINE_TOKEN = 0, PAGE_TOKEN = 1, ADDR_TOKEN = 2, VAL_TOKEN = 3, STATUS_TOKEN = 4};


class VIEW{
public:
    //Constructor
    VIEW(){pageFormat[CURR_PAGE_NUM] = 0; pageFormat[MEM_PAGES] = 1; pageFormat[LINE_SPACE] = PRINT_LINE;};

//Displays a requested page
    void Display(const int& p){
        pageFormat[CURR_PAGE_NUM] = p; //At page 'p'
        currPage.str(menus.getMenu(p)); //Get menu page from library facade

        //Check which page was loaded to put fist page num
        if(p == EDIT || p == EXEC)
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]), to_string(1)+" "));

        else if (p == MAIN) {
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[STATUS_TOKEN]), "  "));
            pageFormat[EXEC_PAGES] = 1;
        }
        if (p == EXEC){
            pageFormat[LINE_SPACE] = PRINT_LINE;
        }

        //Check if loaded page is right resolution of 120x30 chars
        Validate(stringstream(currPage.str()));

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //Display the memory on the load/save page. True = load, False = save
    void DisplayLoadSave(bool menu, MEMORY load){
        currPage.str(menus.getMenu((menu)?LOAD:SAVE)); //Get load menu
        pageFormat[CURR_PAGE_NUM] = (menu)?LOAD:SAVE;

        //Replace lines with data
        for (size_t i = 0; i < 15 && i < load.getMap().size(); ++i) {
            currPage.str(regex_replace(currPage.str(), regex(string(TOKENS[LINE_TOKEN])+"( ){"+
                                       to_string(load.getMap()[i].length()+2)+"}"),
                                       ((i>9)?to_string(i):(" ")+to_string(i))+": "+load.getMap()[i], regex_constants::format_first_only)); //Update line
        }

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //Display status of load or save. true = load, false = save
    void MainError(size_t menu, bool status){
        //Load up main menu
        pageFormat[CURR_PAGE_NUM] = MAIN;
        currPage.str(menus.getMenu(MAIN));

        //Switch case for menu
        if (menu == 1){ //Load
            currPage.str(regex_replace(currPage.str(), regex(string(TOKENS[STATUS_TOKEN] + "( ){" +
                                           string(to_string((status)? (MESSAGES[LOAD_SUCCESS].length()-2):(MESSAGES[LOAD_FAIL].length()-2)) + "}"))),
                                       (status)? MESSAGES[LOAD_SUCCESS] : MESSAGES[LOAD_FAIL]));
            if (status)
                pageFormat[MEM_PAGES] = 0;
        }
        else if(menu == 2){ //Save
            currPage.str(regex_replace(currPage.str(), regex(string(TOKENS[STATUS_TOKEN] + "( ){" +
                                        string(to_string((status)? (MESSAGES[SAVE_SUCCESS].length()-2):(MESSAGES[SAVE_FAIL].length()-2)) + "}"))),
                                       (status)? MESSAGES[SAVE_SUCCESS] : MESSAGES[SAVE_FAIL]));
        }
        else if(menu == 3){
            currPage.str(regex_replace(currPage.str(), regex(string(TOKENS[STATUS_TOKEN] + "( ){" +
                                        to_string(MESSAGES[LOAD_REJECTION].length()-2) + "}")), MESSAGES[LOAD_REJECTION]));
        }
        else{ //Execution mode rejected
            currPage.str(regex_replace(currPage.str(), regex(string(TOKENS[STATUS_TOKEN] + "( ){" +
                                        to_string(MESSAGES[EXECUTION_REJECTION].length()-2) + "}")), MESSAGES[EXECUTION_REJECTION]));
        }

        //Check if loaded page is right resolution of 120x30 chars
        Validate(stringstream(currPage.str()));

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //Display line on screen in Edit Mode
    void DispLine(const string& line){

        if (pageFormat[CURR_PAGE_NUM] != EDIT)//Make sure we are in the right page.
            throw logic_error(string("ERROR: Simulator attempted to display a line in Edit mode while in page "
                                           + to_string(pageFormat[CURR_PAGE_NUM])));

        //If max line has been reached, add new page
        pageFormat[LINE_SPACE]++;
        if (pageFormat[LINE_SPACE] >= HEIGHT){
            pageFormat[MEM_PAGES]++; //Push to page vector
            currPage.str(menus.getMenu(EDIT));
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]),
                                       string(to_string(pageFormat[MEM_PAGES])+((pageFormat[MEM_PAGES]<9)? " ":""))));
            pageFormat[LINE_SPACE] = PRINT_LINE;
        }

        currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+"( ){"+
                                        to_string(line.length()-2)+"}"), line, regex_constants::format_first_only)); //Update line

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //Continues the memory state of Edit Mode
    void ContinueEdit(MEMORY mem){
        if (pageFormat[CURR_PAGE_NUM] != MAIN)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to access Edit Mode not from the main menu, "
                                           "but instead in page " + to_string(pageFormat[CURR_PAGE_NUM])));
        pageFormat[CURR_PAGE_NUM] = EDIT;

        if (!pageFormat[MEM_PAGES]) {
            if (mem.getMap().size() > 9)
                pageFormat[MEM_PAGES] = (mem.getMap().size() / 10)+1;
            else
                pageFormat[MEM_PAGES] = 1;
        }

        currPage.str(menus.getMenu(EDIT)); //Load up the Edit Page
        currPage.str(regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]),
                                   string(to_string(pageFormat[MEM_PAGES])+((pageFormat[MEM_PAGES]<9)? " ":"")))); //Put page no

        size_t start; //What page does the list start populating at?
        if (pageFormat[MEM_PAGES] == 1)
            start = 0;
        else
            start = (pageFormat[MEM_PAGES]-1)*10;

        for(size_t i = start; i < mem.getMap().size(); i++){ // Replace line tokens with memory
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+"( ){"+
                                           to_string((mem.inputMap[i].length()>9)?mem.inputMap[i].length()+1:mem.inputMap[i].length()+2)+"}"),
                                       to_string(i)+": "+mem.inputMap[i], regex_constants::format_first_only)); //Update line
        }

//        if (lineSpace-mem.getMap().size()-start <= mem.getMap().size()-start)
            pageFormat[LINE_SPACE] = PRINT_LINE+(mem.getMap().size()-start); //Compensate for lineSpace

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //User's input is invalid, print message on view
    void DisplayInvalid(){

        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Re-display View
        cout << MESSAGES[INV_MESSAGE];// Display message
        cout.flush();
    }

    //Error encountered during execution mode, print message on view
    //type, line, opcode
    void DisplayError(size_t type, size_t line, size_t opcode){
        if (pageFormat[CURR_PAGE_NUM] != EXEC)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to display an Execution Error while in page "
                                           + to_string(pageFormat[CURR_PAGE_NUM]) + " instead of Execution Page"));

        //If max line has been reached, add new page
        pageFormat[LINE_SPACE]++;
        if (pageFormat[LINE_SPACE] >= HEIGHT){
            pageFormat[EXEC_PAGES]++; //Push to page vector
            currPage.str(menus.getMenu(EXEC));
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]),
                                       string(to_string(pageFormat[EXEC_PAGES])+((pageFormat[EXEC_PAGES]<10)? " ":""))));
            pageFormat[LINE_SPACE] = PRINT_LINE;
        }

        string error;

        //Error type switch case
        switch (type) {
            case SIGN:
                currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN] +
                                            "( ){"+to_string(MESSAGES[SIGN_ERROR].size()-2)+"}"), MESSAGES[SIGN_ERROR],
                                           regex_constants::format_first_only));
                currPage.str(regex_replace(currPage.str(), regex(TOKENS[ADDR_TOKEN]), to_string(line)+((line<9)?"":" ")));
                break;
            case INV_OPCODE:
                currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+
                                            "( ){"+to_string(MESSAGES[INV_ERROR].size()-2)+"}"), MESSAGES[INV_ERROR],
                                           regex_constants::format_first_only));
                currPage.str(regex_replace(currPage.str(), regex(TOKENS[VAL_TOKEN]+
                                            "( ){"+to_string(to_string(opcode).size()-2)+"}"),
                                            to_string(opcode)+((line<9)?" ":"")));
                currPage.str(regex_replace(currPage.str(), regex(TOKENS[ADDR_TOKEN]), to_string(line)+((line<9)?"":" ")));
                break;
            case OVER_FLOW:
                currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+
                                            "( ){"+to_string(MESSAGES[OVER_WARNING].size()-2)+"}"), MESSAGES[OVER_WARNING],
                                           regex_constants::format_first_only));
                break;
            case UNDER_FLOW:
                currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+
                                            "( ){"+to_string(MESSAGES[UNDER_WARNING].size()-2)+"}"), MESSAGES[UNDER_WARNING],
                                           regex_constants::format_first_only));
                break;
            default:
                throw(logic_error(string("CPU threw an unknown error of type " + to_string(type) + "... EXITING SIMULATOR")));
        }

        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }


    //Display message that Halt was reached in execution mode
    void DisplayEnd(){
        if (pageFormat[CURR_PAGE_NUM] != EXEC)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to display the HALT opcode message while in page "
                                     + to_string(pageFormat[CURR_PAGE_NUM]) + " instead of Execution Page"));

        //If max line has been reached, add new page
        pageFormat[LINE_SPACE]++;
        if (pageFormat[LINE_SPACE] >= HEIGHT){
            pageFormat[EXEC_PAGES]++; //Push to page vector
            currPage.str(menus.getMenu(EXEC));
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]), to_string(pageFormat[EXEC_PAGES])+
                                                                          ((pageFormat[EXEC_PAGES]<10)?" ":"")));
            pageFormat[LINE_SPACE] = PRINT_LINE;
        }

        currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+"( ){"+
                                        to_string(MESSAGES[HALT_MESSAGE].size()-2)+"}"), MESSAGES[HALT_MESSAGE],
                                        regex_constants::format_first_only)); //Put read message in page

        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //Display the write message on the Exec page
    void DisplayWrite(const int& addr, const int& value){
        if (pageFormat[CURR_PAGE_NUM] != EXEC)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to display the Write opcode message while in page "
                                            + to_string(pageFormat[CURR_PAGE_NUM]) + " instead of Execution Page"));

        //Make message template
        string output = MESSAGES[WRITE_TEMPLATE];
        output = regex_replace(output, regex(TOKENS[ADDR_TOKEN]), to_string(addr));
        output = regex_replace(output, regex(TOKENS[VAL_TOKEN]), to_string(value));

        //If max line has been reached, add new page
        pageFormat[LINE_SPACE]++;
        if (pageFormat[LINE_SPACE] >= HEIGHT){
            pageFormat[EXEC_PAGES]++; //Push to page vector
            currPage.str(menus.getMenu(EXEC));
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]),
                                       to_string(pageFormat[EXEC_PAGES])+((pageFormat[EXEC_PAGES]<10)?" ":"")));
            pageFormat[LINE_SPACE] = PRINT_LINE;
        }

        currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+"( ){"+
                                        to_string(output.size()-2)+"}"), output, regex_constants::format_first_only)); //Put write message in page
        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //Display the read message on the Exec page
    void DisplayRead(string value){
        if (pageFormat[CURR_PAGE_NUM] != EXEC)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to display the Read opcode message while in page "
                                     + to_string(pageFormat[CURR_PAGE_NUM]) + " instead of Execution Page"));

//        string output;

        //Prompt was given, display their reply on the already printed line
        if (!value.empty()){
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN] + "( ){" +
                                       to_string((stoi(value)<10 && stoi(value)>-1)?value.size()-1:value.size()-2) + "}"),
                                       value, regex_constants::format_first_only)); //Put value in read message
        }

        //Prompt the user for input
        else {
            //If max line has been reached, add new page
            pageFormat[LINE_SPACE]++;
            if (pageFormat[LINE_SPACE] >= HEIGHT) {
                pageFormat[EXEC_PAGES]++; //Push to page vector
                currPage.str(menus.getMenu(EXEC));
                currPage.str(
                        regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]), to_string(pageFormat[EXEC_PAGES]) +
                                           ((pageFormat[EXEC_PAGES] < 10) ? " ": "")));
                pageFormat[LINE_SPACE] = PRINT_LINE;
            }

            currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN] + "( ){" +
                         to_string(MESSAGES[READ_TEMPLATE].size() - 2) + "}"),
                                       MESSAGES[READ_TEMPLATE],
                                       regex_constants::format_first_only)); //Put read message in page

//            output = regex_replace(currPage.str(), regex(TOKENS[VAL_TOKEN]), "  ");//Be sure to shadow the value token
        }
        Validate(stringstream(currPage.str())); //Check if page is valid

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }


private:
    //Index names are enumerated on top
    MenuStrings menus;
    stringstream currPage; //What is currently displayed on the view
    size_t pageFormat[4]; //Holds data for page formatting.

    //CONST string tokens, regex replace tokens
    const string TOKENS[5] = {"%l", "%p" ,"%a", "%v", "%s"};

    //CONST string messages, pre-made template messages
    const string MESSAGES[14] = {"ERROR: Invalid Option, try again. > ",
    "Memory Successfully Loaded!",
    "Memory Could Not Load",
    "Memory Successfully Saved!",
    "Memory Could Not Save",
    "No Save File Detected",
    "No memory has been staged yet",
    "Error: Invalid word sign detected at line %a... Ending Execution",
    "Error: Invalid opcode %v read at address %a... Ending Execution",
    "Warning: Overflow detected",
    "Warning: Underflow detected",
    "Memory location %a currently has %v",
    "Please input a number: %l",
    "HALT Reached. Press ENTER to display memory dump."};


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