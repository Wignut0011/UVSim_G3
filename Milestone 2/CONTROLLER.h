#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "MODEL.h"
#include <fstream>
#include <filesystem>

class CONTROLLER{
    MODEL& model;
    VIEW& view;

public:
    //Constructor
    CONTROLLER(MODEL &m, VIEW &v): model(m), view(v)
    {
        saveFile = "";
        loadFile = "";
    };


    /// TODO: Make sure prompts on main menu/ read me files match up with expected input for page navigation

    /// Start of program
    void StartSimulator()
    {
        //Landing Page
        loadMenu();
    }

    /// Gathers user input for Edit Mode
    bool readUserInput()
    {
        bool done = false;
        // MemSize is used to place input at the end of map to avoid overwriting data
        size_t memSize = model.GetMemory().size();
        //loops until memory is full of user input 5 digit exit code
        for (size_t i = 0; i < (100 - memSize); i++) {
            if (!done)
            {
                string line;
                if (i < 10) line = "0" + to_string(i);
                else line = to_string(i);
                cout << line << " ? "; cout.flush();

                //gather user input
                string uInput;
                cin >> uInput;


                if (uInput.length() == 6)
                {
                    if (uInput == "-99999") // user is done with program and wishes to begin execution
                    {
                        done = true;
                        uInput = "+0000";
                    }
                    else if (uInput == "+99999") // user would like to return to main menu and save their progress
                    {
                        return false;
                    }
                    else // user input an invalid 5 digit exit code
                    {
                        view.DisplayInvalid();
                        i--;
                    }
                }

                    // Check user input for errors
                else if ((uInput[0] != '+') || (uInput.length() != 5) || (uInput[0] != '-'))
                {
                    view.DisplayInvalid();
                    i--;
                }



                    // Send user input to MODEL to store in memory
                else {
                    model.updateMemory(uInput, memSize + i);
                    view.DispLine(uInput);
                }
            }
                // Format rest of empty cells
            else
                // Tell MODEL the remaining cells are empty
                model.updateMemory("+0000", memSize + i);
        }
        return done;
    }

    /// Starts execution mode
    void executeMode()
    {
        if (!model.hasMemory()) {//Can't execute with no memory dummy
            view.MainError(3, false);
        }
        else{
            model.updateMenu(EXEC);
            model.runCPU();

            // After execution, user can enter "1" to return to main menu
            string in;
            cin >> in;
            if (in == "1") loadMenu();
        }
    }


    /// Loads a specified menu page
    void loadMenu()
    {
        model.updateMenu(MAIN);

        string in;
        cin >> in;
        int page = stoi(in);
        switch (page)
        {
            case 1: navigateReadMe(README_1);
                break;
            case 2: model.updateMenu(EDIT);
                if (readUserInput()) executeMode();
                else loadMenu();
                break;
            case 3: load();
                loadMenu();
                break;
            case 4: save();
                loadMenu();
                break;
            case 5: executeMode();
                break;
            case 6: return; //exit(EXIT_SUCCESS) is handled in main as return 0
            default: view.DisplayInvalid(); //It just displays a message, no new page.
                break;
        }
    }

    /// Navigates the Read Me files
    void navigateReadMe(int page)
    {
        bool navigated = false;
        model.updateMenu(page);
        string in;

        while(!navigated){ //Loop input again if invalid selection
            cin >> in;
            int input = stoi(in);
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
            else if (page > 1 && page < 5) {
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
                        view.DisplayInvalid();
                }
            } else if (page == 5 && input == 2) { //Last page, no next button
                navigateReadMe(page - 1);
                navigated = true;
            }
            if (!navigated) {
                view.DisplayInvalid();
            }
        }
    }

    /// Saves the current programs memory to .txt file
    void save()
    {
        model.updateMenu(SAVE);
        cout << "\nSave file as: ";
        cin >> saveFile;

        ofstream file (saveFile);
        if (file.is_open())
        {
            for(int i = 0; i < model.GetMemory().size(); i++)
            {
                file << model.GetMemory()[i] << endl;
            }
            file.close();
            cout << "File saved successfully\n";
        }
        else cout << "Error creating save file!\n";
    }

    /// Loads a specified .txt file into memory
    void load()
    {

        model.updateMenu(LOAD);
        cout << "\nLoad which file?: ";
        cin >> loadFile;

        string line;
        ifstream file (loadFile);
        if(file.is_open())
        {
            int i = 0;
            while(getline(file, line))
            {
                model.updateMemory(line, i++);
            }
            file.close();
            cout << "File loaded successfully\n";
        }
        else cout <<"Unable to open file!\n";
    }

private:
    string saveFile;
    string loadFile;

};
#endif