//Includes
#include "MODEL.h"
#include <fstream>

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
        int memSize = model.memory.inputMap.size();
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
                        view.DisplayError();
                        i--;
                    }
                }

                    // Check user input for errors
                else if ((uInput[0] != '+') || (uInput.length() != 5) || (uInput[0] != '-'))
                {
                    view.DisplayError();
                    i--;
                }



                    // Send user input to MODEL to store in memory
                else model.updateMemory(uInput, memSize + i);
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
        model.updateMenu(EXEC);
        model.runCPU();

        // After execution, user can enter "1" to return to main menu
        string in;
        cin >> in;
        if (in == "1") loadMenu();
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
            case 0: loadMenu();
                break;
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
            case 6: exit(EXIT_SUCCESS);
            case 7: model.updateMenu(DUMP);
                break;
            default: view.DisplayInvalid();
                // user can enter "1" to return to main menu
                string line;
                cin >> line;
                if (line == "1") loadMenu();

        }
    }

    /// Navigates the Read Me files
    void navigateReadMe(int page)
    {
        model.updateMenu(page);
        string in;
        cin >> in;
        int input = stoi(in);
        if (input == 0) loadMenu();
        else if (input >= 1 && input <= 5) navigateReadMe(input);
        else
        {
            view.DisplayInvalid();
            navigateReadMe(page);
        }
    }

    /// Saves the current programs memory to .txt file
    bool save()
    {
        model.updateMenu(SAVE);
        cout << "\nSave file as: ";
        cin >> saveFile;

        ofstream file (saveFile);
        if (file.is_open())
        {
            for(int i = 0; i < model.memory.inputMap.size(); i++)
            {
                file << model.memory.inputMap[i] << endl;
            }
            file.close();
            cout << "File saved successfully\n";
        }
        else cout << "Error creating save file!\n";


    }

    /// Loads a specified .txt file into memory
    bool load()
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