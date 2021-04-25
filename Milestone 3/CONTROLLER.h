#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "MODEL.h"
#include <fstream>

///Anthony Peterson
///Santiago Ramirez - Sub-Menus

class CONTROLLER{
    const string saveFile = "Save.mem";
    string& clipboard;
    MODEL& model;
    VIEW& view;

public:
    //Constructor
    CONTROLLER(MODEL &m, VIEW &v, string &c): model(m), view(v), clipboard(c){}


    // TODO: Make sure prompts on main menu/ read me files match up with expected input for page navigation

    // Start of program
    void StartSimulator()
    {
        //Landing Page
        loadMenu();
    }

    // Gathers user input for Edit Mode
    void readUserInput()
    {
        bool done = false;
        // MemSize is used to place input at the end of map to avoid overwriting data
        size_t memSize = 0;
        if (model.hasMemory())
            memSize = model.GetMemory().getMap().size();

//        size_t start = 0; //What page does the list start populating at?
//        if (model.GetMemory().getMap().size() > 9)
//            start = (model.GetMemory().getMap().size()/10)*10;

        //loops until memory is full of user input 5 digit exit code
        for (size_t i = memSize; i < (100 - memSize); i++) {
            if (!done)
            {
//                string line;
//                if (i < 10) line = "0" + to_string(i);
//                else line = to_string(i);
//                cout << line << " ? "; cout.flush();

                //gather user input
                string uInput;
                cin >> uInput;

                if (uInput.length() == 6) {
                    if (uInput == "-99999") // user is done with program and not to save
                    {
                        view.Display(MAIN);
                        done = true;
//                        uInput = "+0000";
                    } else if (uInput == "+99999") // user would like to return to main menu and save their progress
                    {
//                        return false;
                        if (!model.hasMemory())
                            view.MainError(2, false);
                        else {
                            ofstream save(saveFile);
                            if (save) {
                                for (int i = 0; i < model.GetMemory().getMap().size(); i++) {
                                    save << model.GetMemory().getMap()[i] << "\n";
                                }
                                view.MainError(2, true);
                            } else
                                view.MainError(2, false);
                            save.close();

                        }
                        done = true;
                    }
                }

                ///Santiago Ramirez - Sub-Menus

                else if(uInput.length() == 1 && isdigit(uInput[0]) &&
                    (stoi(uInput) >= ED_COPY-10 && stoi(uInput) <= ED_DELETE-10)) { //Sub-menu

                    //Check if it has memory
                    string lineNum;
                    switch (stoi(uInput)) {
                        case 1: { //Copy
                            if (!model.hasMemory()) //Is the clipboard empty
                                view.DisplayInvalid(SUB_REJ_MESSAGE);

                            else {
                                view.Display(subPages::ED_COPY);
                                while (lineNum.empty()) { //Stay in sub-menu until selection
                                    //get the selected lineNum
                                    while (lineNum.empty()) {
                                        cin >> lineNum;
                                        bool pass = true;
                                        for (auto l : lineNum)//Check if contains only digits
                                            if (!isdigit(l))
                                                pass = false;
                                        if ((lineNum.length() != 2 && lineNum.length() != 1) || !pass) { //Not valid
                                            view.DisplayInvalid(INV_MESSAGE);
                                            lineNum = "";
                                        }
                                    }
                                    if (lineNum.length() == 1) {
                                        switch (stoi(lineNum)) {
                                            case 1: {
                                                view.Display(subPages::NEXT_SUB);
                                                lineNum = "";
                                                break;
                                            }
                                            case 2: {
                                                view.Display(subPages::PREV_SUB);
                                                lineNum = "";
                                                break;
                                            }
                                            case 3: {
                                                view.ContinueEdit(model.GetMemory());
                                            }
                                        }
                                    }
                                    else if (stoi(lineNum) < model.GetMemory().getMap().size()){
                                        clipboard = model.GetMemory().getMap().at(stoi(lineNum));
                                        view.ContinueEdit(model.GetMemory());
                                        break;
                                    }
                                    else {
                                        view.DisplayInvalid(INV_MESSAGE);
                                        lineNum = "";
                                    }
                                }
                            }
                            break;
                        }
                        case 2: { //Cut
                            if (!model.hasMemory()) //Is there any memory yet
                                view.DisplayInvalid(SUB_REJ_MESSAGE);

                            else {
                                view.Display(subPages::ED_CUT);
                                while (lineNum.empty()) { //Stay in sub-menu until selection
                                    //get the selected lineNum
                                    while (lineNum.empty()) {
                                        cin >> lineNum;
                                        bool pass = true;
                                        for (auto l : lineNum)//Check if contains only digits
                                            if (!isdigit(l))
                                                pass = false;
                                        if ((lineNum.length() != 2 && lineNum.length() != 1) || !pass) { //Not valid
                                            view.DisplayInvalid(INV_MESSAGE);
                                            lineNum = "";
                                        }
                                    }
                                    if (lineNum.length() == 1) {
                                        switch (stoi(lineNum)) {
                                            case 1: {
                                                view.Display(subPages::NEXT_SUB);
                                                lineNum = "";
                                                break;
                                            }
                                            case 2: {
                                                view.Display(subPages::PREV_SUB);
                                                lineNum = "";
                                                break;
                                            }
                                            case 3: {
                                                view.ContinueEdit(model.GetMemory());
                                            }
                                        }
                                    }
                                    else if (stoi(lineNum) < model.GetMemory().getMap().size()){
                                        clipboard = model.GetMemory().getMap().at(stoi(lineNum)); //gives a string
                                        for (size_t j = stoi(lineNum);j != model.GetMemory().getMap().size() - 1; j++) {
                                            model.GetMemory().getMap().at(j) = model.GetMemory().getMap().at(j + 1);
                                        }
                                        model.GetMemory().getMap().erase(model.GetMemory().getMap().size()-1);
                                        view.ContinueEdit(model.GetMemory());
                                        break;
                                    }
                                    else {
                                        view.DisplayInvalid(INV_MESSAGE);
                                        lineNum = "";
                                    }
                                }
                            }
                            break;
                        }
                        case 3: { //Paste
                            if (clipboard.empty()) //Is the clipboard empty
                                view.DisplayInvalid(SUB_CLIP_MESSAGE);

                            else if (!model.hasMemory()){ //Is there any memory yet, just paste
                                model.updateMemory(clipboard, 0);
                                view.DispLine(to_string(i)+": "+clipboard);
                            }

                            else {
                                view.Display(subPages::ED_PASTE);
                                while (lineNum.empty()) { //Stay in sub-menu until selection
                                    //get the selected lineNum
                                    while (lineNum.empty()) {
                                        cin >> lineNum;
                                        bool pass = true;
                                        for (auto l : lineNum)//Check if contains only digits
                                            if (!isdigit(l))
                                                pass = false;
                                        if ((lineNum.length() != 2 && lineNum.length() != 1) || !pass) { //Not valid
                                            view.DisplayInvalid(INV_MESSAGE);
                                            lineNum = "";
                                        }
                                    }
                                    if (lineNum.length() == 1) {
                                        switch (stoi(lineNum)) {
                                            case 1: {
                                                view.Display(subPages::NEXT_SUB);
                                                lineNum = "";
                                                break;
                                            }
                                            case 2: {
                                                view.Display(subPages::PREV_SUB);
                                                lineNum = "";
                                                break;
                                            }
                                            case 3: {
                                                view.ContinueEdit(model.GetMemory());
                                            }
                                        }
                                    }
                                    else if (stoi(lineNum) < model.GetMemory().getMap().size()){
                                        model.updateMemory(clipboard, stoi(lineNum));
                                        view.ContinueEdit(model.GetMemory());
                                        break;
                                    }
                                    else {
                                        view.DisplayInvalid(INV_MESSAGE);
                                        lineNum = "";
                                    }
                                }
                            }
                            break;
                        }
                        case 4: { //Insert whatever is on the clipboard.
                            if (clipboard.empty()) //Is the clipboard empty
                                view.DisplayInvalid(SUB_CLIP_MESSAGE);

                            else if (!model.hasMemory()) { //Is there any memory yet
                                model.updateMemory(clipboard, 0);
                                view.DispLine(to_string(i)+": "+clipboard);
                            }

                            else {
                                view.Display(subPages::ED_INSERT);
                                while (lineNum.empty()) { //Stay in sub-menu until selection
                                    while (lineNum.empty()) {
                                        cin >> lineNum;
                                        bool pass = true;
                                        for (auto l : lineNum)//Check if contains only digits
                                            if (!isdigit(l))
                                                pass = false;
                                        if ((lineNum.length() != 2 && lineNum.length() != 1) || !pass) { //Not valid
                                            view.DisplayInvalid(INV_MESSAGE);
                                            lineNum = "";
                                        }
                                    }
                                    if (lineNum.length() == 1) {
                                        switch (stoi(lineNum)) {
                                            case 1: {
                                                view.Display(subPages::NEXT_SUB);
                                                lineNum = "";
                                                break;
                                            }
                                            case 2: {
                                                view.Display(subPages::PREV_SUB);
                                                lineNum = "";
                                                break;
                                            }
                                            case 3: {
                                                view.ContinueEdit(model.GetMemory());
                                            }
                                        }
                                    }
                                    else if (stoi(lineNum) < model.GetMemory().getMap().size()){
                                        model.GetMemory().getMap()[model.GetMemory().getMap().size()] = model.GetMemory().getMap()[
                                                model.GetMemory().getMap().size() - 1];
                                        for (size_t j = model.GetMemory().getMap().size() - 1;
                                             j != stoi(lineNum); j--) {
                                            model.GetMemory().getMap().at(j) = model.GetMemory().getMap().at(j - 1);
                                        }
                                        model.updateMemory(clipboard, stoi(lineNum));
                                        view.ContinueEdit(model.GetMemory());
                                        break;

                                    }
                                    else {
                                        view.DisplayInvalid(INV_MESSAGE);
                                        lineNum = "";
                                    }
                                }
                            }
                            break;
                        }
                        case 5: { //Delete request
                            if (!model.hasMemory()) //Is there any memory yet
                                view.DisplayInvalid(SUB_REJ_MESSAGE);

                            else {
                                view.Display(subPages::ED_DELETE);
                                while (lineNum.empty()) { //Stay in sub-menu until selection
                                    while (lineNum.empty()) {
                                        cin >> lineNum;
                                        bool pass = true;
                                        for (auto l : lineNum)//Check if contains only digits
                                            if (!isdigit(l))
                                                pass = false;
                                        if ((lineNum.length() != 2 && lineNum.length() != 1) || !pass) { //Not valid
                                            view.DisplayInvalid(INV_MESSAGE);
                                            lineNum = "";
                                        }
                                    }
                                     if(lineNum.length() == 1) {
                                         switch (stoi(lineNum)) {
                                             case 1: {
                                                 view.Display(subPages::NEXT_SUB);
                                                 lineNum = "";
                                                 break;
                                             }
                                             case 2: {
                                                 view.Display(subPages::PREV_SUB);
                                                 lineNum = "";
                                                 break;
                                             }
                                             case 3: {
                                                 view.ContinueEdit(model.GetMemory());
                                                 break;
                                             }
                                         }
                                     }
                                     else if (stoi(lineNum) < model.GetMemory().getMap().size()){
                                         if (model.GetMemory().inputMap.size() == 1) //Just one entry
                                             model.GetMemory().getMap().erase(0);
                                         else {
                                             for (size_t j = stoi(lineNum); j != model.GetMemory().getMap().size() - 1; j++) {
                                                 model.GetMemory().getMap().at(j) = model.GetMemory().getMap().at(j + 1);
                                             }
                                             model.GetMemory().getMap().erase(model.GetMemory().getMap().size()-1);
                                         }
                                         view.ContinueEdit(model.GetMemory());
                                         break;
                                     }
                                     else {
                                         view.DisplayInvalid(INV_MESSAGE);
                                         lineNum = "";
                                     }
                                }
                            }
                            break;
                        }
                    }
                    i = model.GetMemory().inputMap.size()-1;

//                    if (model.GetMemory().inputMap.empty()) //If empty, start at beginning
//                        i -= 1;
//                    else //Not empty, restart at next position
//                        i -= 2;
                }
                ///Santiago Ramirez End

                // Check user input for errors
                else if ((uInput[0] != '+' && uInput[0] != '-') || uInput.length() != 5)
                {
                    view.DisplayInvalid(INV_MESSAGE);
                    i--;
                }

                    // Send user input to MODEL to store in memory
                else {
                    model.updateMemory(uInput, i);
                    view.DispLine(to_string(i)+": "+uInput); //Insert line number for view
                }
            }
                // Format rest of empty cells
//            else
                // Tell MODEL the remaining cells are empty
//                model.updateMemory("+0000", i);
        }
//        return done;
    }

    // Starts execution mode
    void executeMode()
    {
        if (!model.hasMemory()) {//Can't execute with no memory dummy
            view.MainError(4, false);
            return;
        }
        else{
            model.updateMenu(EXEC);
            model.runCPU();
            view.Display(MAIN);

//            // After execution, user can enter "1" to return to main menu
//            string in;
//            cin >> in;
//            if (in == "1") loadMenu();
        }
    }


    // Loads a specified menu page
    void loadMenu()
    {
        model.updateMenu(MAIN);

        string in;
        int page;
        while(true){
            while (in.empty()) { //Make sure user inputs a number
                cin >> in;
                if (in != "1" && in != "2" && in != "3" && in != "4" && in != "5" && in != "6") {
                    in = "";
                    view.DisplayInvalid(INV_MESSAGE);
                } else page = stoi(in);
            }

            switch (page) {
                case 1:
                    navigateReadMe(README_1);
                    break;
                case 2:
                    model.updateMenu(EDIT);
                    readUserInput();
//                    if (readUserInput()) executeMode();
//                    else model.updateMenu(MAIN);
                    break;
                case 3:
                    load();
//                    model.updateMenu(MAIN);
                    break;
                case 4:
                    if (model.hasMemory())
                        save();
                    else(view.MainError(4, false));
//                    model.updateMenu(MAIN);
                    break;
                case 5:
                    executeMode();
                    break;
                case 6:
                    exit(EXIT_SUCCESS); //exit will be handled here. If in main, exit return is 1
                    break;
                default:
                    view.DisplayInvalid(INV_MESSAGE); //It just displays a message, no new page.
                    break;
            }
            in = "";
        }
    }

    // Navigates the Read Me files
    void navigateReadMe(int page)
    {
        bool navigated = false;
        model.updateMenu(page);
        int input;
        string in;

        while(!navigated){ //Loop input again if invalid selection
            while (in.empty()) { //Make sure user inputs a number
                cin >> in;
                if (in != "1" && in != "2" && in != "3") {
                    in = "";
                    view.DisplayInvalid(INV_MESSAGE);
                } else input = stoi(in);
            }
            if (input == 1) { //Return to main menu
                loadMenu();
                navigated = true;
            }

            //First page, no prev option
            else if (page == 1 && input == 2){
                navigateReadMe(page+1);
                navigated= true;
            }
            //Middle pages, has next and previous options
            else if (page > 1 && page < 6) {
                switch (input) {
                    case 2:
                        navigateReadMe(page + 1); //Next Page
                        navigated = true;
                        break;
                    case 3:
                        navigateReadMe(page - 1); //Previous Page
                        navigated = true;
                        break;
                    default:
                        view.DisplayInvalid(INV_MESSAGE);
                }
            } else if (page == 6 && input == 2) { //Last page, no next button
                navigateReadMe(page - 1);
                navigated = true;
            }
            if (!navigated) {
                view.DisplayInvalid(INV_MESSAGE);
            }
        }
    }

    // Saves the current programs memory to .txt file
    void save()
    {
        view.DisplayLoadSave(false, model.GetMemory()); //Display Save Page

        string answ;
//        else {
//            view.MainError(2, false);
//            return;
//        }
//        model.updateMenu(SAVE);

        while (answ.empty()) {
            while (answ.empty()) { //Make sure user inputs a number
                cin >> answ;
                if (answ != "1" && answ != "2") {
                    answ = "";
                    view.DisplayInvalid(INV_MESSAGE);
                }
            }
            if (stoi(answ))
                switch (stoi(answ)) {
                    case 1: //Save memory
                    {
                        ofstream file(saveFile, ios::trunc);
                        if (file.is_open()) {
                            for (int i = 0; i < model.GetMemory().getMap().size(); i++) {
                                file << model.GetMemory().getMap()[i] << "\n";
                            }
                            file.close();
                            view.MainError(2, true);
                        } else
                            view.MainError(2, false);
                    }
                        break;
                    case 2: //Don't save
                        view.Display(MAIN);
                        break;
                    default:
                        answ = "";
                }
            else
                answ = "";
        }
    }

    // Loads a specified .txt file into memory
    void load()
    {
        //Check if there is a file to load
        ifstream save(saveFile, ios::in);
        if (!save){
            view.MainError(3, false);
            save.close();
//            loadMenu();
            return;
        }
        save.close();

        MEMORY load;
        string line;
        ifstream file (saveFile);
        if(file.is_open()){
            int i = 0;
            while(getline(file, line))
            {
                load.add(line, i++);
            }
            file.close();
            view.DisplayLoadSave(true, load); //Display load screen

            //Ask if user wants to load save
            string answ;
            while (answ.empty()) {
                cin >> answ;
                if (answ != "1" && answ != "2" && answ != "3"){
                    answ = "";
                    view.DisplayInvalid(INV_MESSAGE);
                }
                else
                    switch (stoi(answ)) {
                        case 1:
                            model.loadMemory(load);
                            view.MainError(1, true);
                            break;
                        case 2:
                            view.Display(MAIN);
                            break;
                        case 3:
                            model.loadMemory(MEMORY());
                            view.Display(MAIN);
                            break;
                    }

            }
//            cout << "File loaded successfully\n";
        }
//        else cout <<"Unable to open file!\n";
        else view.MainError(1, false);
    }
};
#endif