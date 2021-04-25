#ifndef VIEW_H
#define VIEW_H
#include "MenuStrings.h"
#include "MEMORY.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

///Daniel Espinel

//Enumerations, these are not attributes.
//Resolution line consts
enum resolution :size_t     {WIDTH = 120, HEIGHT = 30, PRINT_LINE = 19};

//Error types thrown by CPU
enum errorTypes :size_t     {SIGN = 1, INV_OPCODE = 2, OVER_FLOW = 3, UNDER_FLOW = 4};

//Error message types
enum messageType :size_t    {INV_MESSAGE = 0, LOAD_SUCCESS = 1, LOAD_FAIL = 2, SAVE_SUCCESS = 3, SAVE_FAIL = 4,
                             LOAD_REJECTION = 5, EXECUTION_REJECTION = 6, SIGN_ERROR = 7, INV_ERROR = 8,
                             OVER_WARNING = 9, UNDER_WARNING = 10, WRITE_TEMPLATE = 11, READ_TEMPLATE = 12,
                             HALT_MESSAGE = 13, SUB_REJ_MESSAGE = 14, SUB_CLIP_MESSAGE = 15};

//Main Menu page numbers
enum mainPages :size_t      {MAIN = 0, README_1 = 1, README_2 = 2, README_3 = 3, README_4 = 4, README_5 = 5, README_6 = 6,
                             EDIT = 7, EXEC = 8, SAVE = 9, LOAD = 10};

//Sub_Menu page numbers
enum subPages :size_t       {ED_COPY = 11, ED_CUT = 12, ED_PASTE = 13, ED_INSERT = 14, ED_DELETE = 15, NEXT_SUB = 16,
                             PREV_SUB = 17};

//pageFormat data structure index names
enum formatting :size_t     {CURR_PAGE_NUM = 0, EXEC_PAGES = 1, MEM_PAGES = 2, LINE_SPACE = 3, CURR_MEM_PAGE = 4};

//Index names for data structure storing Regex Tokens
enum tokenIndex :size_t     {LINE_TOKEN = 0, PAGE_TOKEN = 1, ADDR_TOKEN = 2, VAL_TOKEN = 3, STATUS_TOKEN = 4,
                             CLIP_TOKEN = 5};


class VIEW{
public:
    //Constructor
    VIEW(string& c): clipboard(c){pageFormat[CURR_PAGE_NUM] = 0;pageFormat[MEM_PAGES] = 1;pageFormat[LINE_SPACE] = PRINT_LINE;}

//Displays a requested page
    void Display(const int& p) {
        //A main menu page
        if (p >= mainPages::MAIN && p <= mainPages::LOAD) {
            pageFormat[CURR_PAGE_NUM] = p; //At page 'p'
            currPage.str(menus.getMenu(p)); //Get menu page from library facade

            //Check which page was loaded to put fist page num
            if (p == EDIT || p == EXEC) {
                if (p == EDIT) { //If Edit was reached here, that means that there should be no Memory
                    memCache.clear();
                    pageFormat[MEM_PAGES] = 1;
                    pageFormat[CURR_MEM_PAGE] = 1;
                }
                currPage.str(regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]), to_string(1) + " "));
            }

            else if (p == MAIN) {
                currPage.str(regex_replace(currPage.str(), regex(TOKENS[STATUS_TOKEN]), "  "));
                pageFormat[EXEC_PAGES] = 1;
            }
            if (p == EXEC) {
                pageFormat[LINE_SPACE] = PRINT_LINE;
            }

            //Check if loaded page is right resolution of 120x30 chars
            Validate(stringstream(currPage.str()));

            cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
            cout.flush();
        }

        //A Sub-Menu Page
        else if(p >= subPages::ED_COPY && p <=subPages::PREV_SUB) {
            string output;

            //Page limit
            if ((p == NEXT_SUB && pageFormat[CURR_MEM_PAGE] >= memCache.size()) ||
                (p == PREV_SUB && pageFormat[CURR_MEM_PAGE] <= 1)) {
                DisplayInvalid(INV_MESSAGE);
                return;
            }

            //No memory, print invalid
            if (memCache.empty()){
                DisplayInvalid(SUB_REJ_MESSAGE);
                return;
            }

            //Loading next memory page
            if (p == NEXT_SUB || p == PREV_SUB) {

                //Make sure page reload is accurate
                output = menus.getMenu(pageFormat[CURR_PAGE_NUM]);

                //Update current page
                (p == NEXT_SUB) ? pageFormat[CURR_MEM_PAGE]++ : pageFormat[CURR_MEM_PAGE]--;

                //Populate with saved data
                string line;
                for (auto l : memCache[pageFormat[CURR_MEM_PAGE]-1]) { //Populate list starting at last page
                    if (l != '\n') //Add char to line
                        line += l;

                    else { //Add line to page
                        output = regex_replace(output, regex(string(TOKENS[LINE_TOKEN]) + "( ){" +
                                                             to_string(line.size()-2) + "}"), line,
                                               regex_constants::format_first_only);
                        line = "";
                    }
                }
                //Make sure to print last line
                if (!line.empty())
                    output = regex_replace(output, regex(string(TOKENS[LINE_TOKEN]) + "( ){" +
                                                         to_string(line.size()-2) + "}"), line,
                                           regex_constants::format_first_only);
            }

            //Loading a sub-menu
            else {
                //Load sub-menu
                output = menus.getMenu(p);
                pageFormat[CURR_PAGE_NUM] = p;

                //Populate list starting at last page
                string line;
                for (auto l : memCache[pageFormat[MEM_PAGES]-1]) {
                    if (l != '\n') //Add char to line
                        line += l;

                    else { //Add line to page
                        output = regex_replace(output, regex(string(TOKENS[LINE_TOKEN]) + "( ){" +
                                                             to_string(line.size()-2) + "}"), line,
                                               regex_constants::format_first_only);
                        line = "";
                    }
                }
                //Make sure to print last line
                if (!line.empty())
                    output = regex_replace(output, regex(string(TOKENS[LINE_TOKEN]) + "( ){" +
                                                     to_string(line.size()-2) + "}"), line,
                                       regex_constants::format_first_only);
            }

            //Populate page no
            output = regex_replace(output, regex(TOKENS[PAGE_TOKEN]), to_string(pageFormat[CURR_MEM_PAGE]) +
                                                                      ((pageFormat[CURR_MEM_PAGE] < 10) ? " " : ""));

            //Does sub-menu need clipboard displayed?
            if (pageFormat[CURR_PAGE_NUM] == ED_INSERT || pageFormat[CURR_PAGE_NUM] == ED_PASTE) {
                output = regex_replace(output,regex(string(TOKENS[CLIP_TOKEN])+"( ){"+
                                                    to_string(clipboard.size()-2)+"}"), clipboard);
            }

            //Save to currPage
            currPage.str(output);

            cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
            cout.flush();
        }

        else //Not a valid menu number, throw
            throw logic_error(string("Simulator attempted to access menu '" +
                                     to_string(pageFormat[CURR_PAGE_NUM]) + "' that does not exist, "));
    }

    //Display the memory on the load/save page. True = load, False = save
    void DisplayLoadSave(bool menu, MEMORY load){
        //Get load menu
        currPage.str(menus.getMenu((menu)?LOAD:SAVE));
        pageFormat[CURR_PAGE_NUM] = (menu)?LOAD:SAVE;

        //Replace lines with data
        for (size_t i = 0; i < 15 && i < load.getMap().size(); ++i) {
            currPage.str(regex_replace(currPage.str(), regex(string(TOKENS[LINE_TOKEN])+"( ){"+
                                       to_string(load.getMap()[i].length()+2)+"}"),
                                       ((i>9)?to_string(i):(" ")+to_string(i))+": "+load.getMap()[i],
                                       regex_constants::format_first_only)); //Update line
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
        else if(menu == 3){ //Load
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
    void DispLine(string line){
        if (pageFormat[CURR_PAGE_NUM] != EDIT)//Make sure we are in the right page.
            throw logic_error(string("ERROR: Simulator attempted to display a line in Edit mode while in page "
                                           + to_string(pageFormat[CURR_PAGE_NUM])));

        //If max line has been reached, add new page
        NewPage(MEM_PAGES, EDIT);

        //Update memCache
        if (line[1] == ':') //Make sure to format right
            line.insert(0,"0");
        if (pageFormat[MEM_PAGES] > memCache.size()) //New page was added
            memCache.emplace_back(line);
        else
            memCache.back().append("\n"+line);

        //Update line
        currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+"( ){"+
                                        to_string(line.length()-2)+"}"), line, regex_constants::format_first_only));

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //Continues the memory state of Edit Mode
    void ContinueEdit(MEMORY mem){
        if ((pageFormat[CURR_PAGE_NUM] != MAIN) && (pageFormat[CURR_PAGE_NUM] < ED_COPY || pageFormat[CURR_PAGE_NUM] > PREV_SUB))//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to access Edit Mode not from main menu or sub-menu, "
                                     "but instead in page " + to_string(pageFormat[CURR_PAGE_NUM])));

        //Reset mem page curr
        pageFormat[MEM_PAGES] = 0;
        memCache.clear();

        //Reset memCache
        for (auto l : mem.getMap()) {
            if (l.first % 10 == 0) { //Append new page
                pageFormat[MEM_PAGES]++;
                memCache.emplace_back((l.first<=9)?
                    ("0"+to_string(l.first)+": "+l.second):(to_string(l.first)+": "+l.second));
            }

            else{ //Insert to current page
                memCache.back().append((l.first<10)?
                                       ("\n0"+to_string(l.first)+": "+l.second):("\n"+to_string(l.first)+": "+l.second));
            }
        }
        //Update pageFormat data
        pageFormat[CURR_MEM_PAGE] = pageFormat[MEM_PAGES];
        pageFormat[CURR_PAGE_NUM] = EDIT;
        pageFormat[EXEC_PAGES] = 1;
        if (!pageFormat[MEM_PAGES]) {
            if (mem.getMap().size() > 9)
                pageFormat[MEM_PAGES] = (mem.getMap().size() / 10)+1;
            else
                pageFormat[MEM_PAGES] = 1;
        }

        //Load up the Edit Page
        currPage.str(menus.getMenu(EDIT));
        currPage.str(regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]),
                                   string(to_string(pageFormat[MEM_PAGES])+((pageFormat[MEM_PAGES]<9)? " ":"")))); //Put page no

        //What page does the list start populating at?
        size_t start;
        if (pageFormat[MEM_PAGES] == 1)
            start = 0;
        else
            start = (pageFormat[MEM_PAGES]-1)*10;

        //Populate list starting at last page
        string line;

        if (!memCache.empty()){
            for (auto l : memCache[pageFormat[MEM_PAGES] - 1]) {
                if (l != '\n') //Add char to line
                    line += l;

                else { //Add line to page
                    currPage.str(regex_replace(currPage.str(), regex(string(TOKENS[LINE_TOKEN]) + "( ){" +
                                                                     to_string(line.size() - 2) + "}"), line,
                                               regex_constants::format_first_only));
                    line = "";
                }
            }
            if (!line.empty()) //Add last line
                currPage.str(regex_replace(currPage.str(), regex(string(TOKENS[LINE_TOKEN]) + "( ){" +
                                                                 to_string(line.size() - 2) + "}"), line,
                                           regex_constants::format_first_only));
        }

        //Compensate for lineSpace
        pageFormat[LINE_SPACE] = PRINT_LINE+(mem.getMap().size()-start);

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //User's input is invalid, print message on view
    void DisplayInvalid(size_t invCode){
        //Check if page is valid
        Validate(stringstream(currPage.str()));

        //Re-display View
        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  ");
        cout << MESSAGES[invCode];// Display message
        cout.flush();
    }

    //Error encountered during execution mode, print message on view
    //type, line, opcode
    void DisplayError(size_t type, size_t line, size_t opcode){
        if (pageFormat[CURR_PAGE_NUM] != EXEC)//Make sure we are in the right page.
            throw logic_error(string("Simulator attempted to display an Execution Error while in page "
                                           + to_string(pageFormat[CURR_PAGE_NUM]) + " instead of Execution Page"));

        //If max line has been reached, add new page
        NewPage(EXEC_PAGES, EXEC);

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
            default: //Unknown Error, Throw
                throw(logic_error(string("CPU threw an unknown error of type " + to_string(type) + "... EXITING SIMULATOR")));
        }

        //Check if page is valid
        Validate(stringstream(currPage.str()));

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }


    //Display message that Halt was reached in execution mode
    void DisplayEnd(){
        //Make sure we are in the right page.
        if (pageFormat[CURR_PAGE_NUM] != EXEC)
            throw logic_error(string("Simulator attempted to display the HALT opcode message while in page "
                                     + to_string(pageFormat[CURR_PAGE_NUM]) + " instead of Execution Page"));

        //If max line has been reached, add new page
        NewPage(EXEC_PAGES, EXEC);

        //Put read message in page
        currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+"( ){"+
                                        to_string(MESSAGES[HALT_MESSAGE].size()-2)+"}"), MESSAGES[HALT_MESSAGE],
                                        regex_constants::format_first_only));

        //Check if page is valid
        Validate(stringstream(currPage.str()));

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //Display the write message on the Exec page
    void DisplayWrite(const int& addr, const int& value){
        //Throw if reached by an invalid menu
        if (pageFormat[CURR_PAGE_NUM] != EXEC)
            throw logic_error(string("Simulator attempted to display the Write opcode message while in page "
                                            + to_string(pageFormat[CURR_PAGE_NUM]) + " instead of Execution Page"));

        //Make message template
        string output = MESSAGES[WRITE_TEMPLATE];
        output = regex_replace(output, regex(TOKENS[ADDR_TOKEN]), to_string(addr));
        output = regex_replace(output, regex(TOKENS[VAL_TOKEN]), to_string(value));

        //If max line has been reached, add new page
        NewPage(EXEC_PAGES, EXEC);

        //Put write message in page
        currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]+"( ){"+
                                        to_string(output.size()-2)+"}"), output, regex_constants::format_first_only));

        //Check if page is valid
        Validate(stringstream(currPage.str()));

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }

    //Display the read message on the Exec page
    void DisplayRead(string value){
        //Throw if reached by an invalid menu
        if (pageFormat[CURR_PAGE_NUM] != EXEC)
            throw logic_error(string("Simulator attempted to display the Read opcode message while in page "
                                     + to_string(pageFormat[CURR_PAGE_NUM]) + " instead of Execution Page"));

        //Prompt was given, display their reply on the already printed line
        if (!value.empty()){
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN] + "( ){" +
                                       to_string((stoi(value)<10 && stoi(value)>-1)?value.size()-1:value.size()-2) + "}"),
                                       value, regex_constants::format_first_only)); //Put value in read message
        }

        //Prompt the user for input
        else{
            //If max line has been reached, add new page
            NewPage(EXEC_PAGES, EXEC);

            currPage.str(regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN] + "( ){" +
                         to_string(MESSAGES[READ_TEMPLATE].size() - 2) + "}"),
                                       MESSAGES[READ_TEMPLATE],
                                       regex_constants::format_first_only)); //Put read message in page
        }

        //Check if page is valid
        Validate(stringstream(currPage.str()));

        cout << regex_replace(currPage.str(), regex(TOKENS[LINE_TOKEN]), "  "); //Update View
        cout.flush();
    }


private:
    //Index names are enumerated on top of VIEW.h
    string& clipboard; //Only real global coupled data
    MenuStrings menus;
    stringstream currPage; //What is currently displayed on the view
    size_t pageFormat[5]; //Holds data for page formatting.
    vector<string> memCache; //Stores the memory locally in page format

    //CONST Regex Tokens
    const string TOKENS[6] = {"%l", "%p" ,"%a", "%v", "%s", "%c"};

    //CONST pre-made messages (Some templates)
    const string MESSAGES[16] = {"ERROR: Invalid Option, try again. > ",
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
    "HALT Reached. Press ENTER to display memory dump.",
    "Please input at least one line before using that >",
    "Clipboard is empty >"};


    //Check if the currPage stream is valid for the console
    void Validate(stringstream check){
        string lines[29]; //Max for default console resolution

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

    //If max line has been reached, add new page
    void NewPage(size_t formatIndex, size_t menuNo) {
        //Update pageFormat tracking the line number
        pageFormat[LINE_SPACE]++;

        //If exceeded line limit, put new page
        if (pageFormat[LINE_SPACE] >= HEIGHT) {
            //Check if MemPages are not out of bounds
            if (formatIndex == MEM_PAGES && pageFormat[MEM_PAGES] > 10)
                throw logic_error("VIEW attempted to add a new page to it's memory page indexing beyond the 10 page limit.");

            pageFormat[formatIndex]++; //Push to page vector
            if (formatIndex == MEM_PAGES)//Make sure to update CUR_MEM_PAGE if checking that
                pageFormat[CURR_MEM_PAGE] = pageFormat[MEM_PAGES];
            currPage.str(menus.getMenu(menuNo));
            currPage.str(regex_replace(currPage.str(), regex(TOKENS[PAGE_TOKEN]),
                                       to_string(pageFormat[formatIndex]) +
                                       ((pageFormat[formatIndex] < 10) ? " ": "")));
            pageFormat[LINE_SPACE] = PRINT_LINE;
        }
    }
};
#endif