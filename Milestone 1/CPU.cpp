//
// Created by santi on 2/5/2021.
//
//
// Created by santi on 2/5/2021.
//
#include <map>
#include <iostream>
#include <string>

using namespace std;

class CPU {
    int accumulator; //The singular register
    string userNum; //String for user I/O prompts
    map<int, string> memory; //Our memory <line#, instruction>

public:
    //----------------
    // EXECUTION MODE
    //----------------

    //create loop that takes input and performs function based on input
    for(int i = 0; i < 100; i++){


    //Constructor
    CPU(map<int,string> inputMap): memory(std::move(inputMap)) {

        //For loop to imitate a cpu clock
        for (int i = 0; i < 100; i++) {
            size_t opcode;
            int operand;

            string line = memory.at(i);
            //stop gathering if user types -99999
            if (line == "-99999") break;

            //==========================================================
            //   Gets the instruction code to be used in the switch
            opcode = abs(stoi(line.substr(0,1))); //Extract opcode substring
            //==========================================================
            //   Gets the data code to be used in the switch
            operand = stoi(line.substr(2)); //Extract operand substring
            //=========================================================
            //   Possibly a code block to find the location of each
            //      operand as we are going through the code.
            //=========================================================

            //switch/case for each instruction
            switch (opcode) {
                case 10:
                    //Read();
                    //1007 = grab first input from the user and put it into desired memory location
                    cout << "Enter an integer: ";
                    cin >> userNum;
                    inputMap[operand] = userNum;
                    cout << "read";
                    break;
                case 11:
                    //Write();
                    //write command; take memory location 09 and give it to the screen to print.
                    cout << "Contents of " << operand << "is " << inputMap[operand];
                    cout << "Write";
                    break;
                case 20:
                    //Load();    Load a word from a specific location in memory into the accumulator
                    //load command; integer from location 07 is loaded into accumulator
                    accumulator = stoi(inputMap[operand]);
                    cout << "Load";
                    break;
                case 21:
                    //Store();   Store a word from the accumulator into a specific location in memory
                    //store command; take the added number and store it in the memory location 09
                    inputMap[operand] = to_string(accumulator);
                    cout << "Store";
                    break;
                case 30:
                    //Add();
                    cout << "Add";
                    break;
                case 31:
                    //Subtract();
                    cout << "Subtract";
                    break;
                case 32:
                    //Divide();
                    cout << "Divide";
                    break;
                case 33:
                    //Multiply();
                    cout << "Multiply";
                    break;
                    //Branch

                //BRANCH
                case 40:
                    //Branch to address in operand
                    i = abs(operand) - 1;
                    break;

                //BRANCHNEG
                case 41:
                    if (accumulator < 0) //If accumulator is negative, branch to address
                        i = abs(operand) - 1;
                    break;

                //BRANCHZERO
                case 42:
                    switch (accumulator) {
                        case 0: //If accumulator is 0, branch to address
                            i = abs(operand) - 1;
                            break;

                        default:
                            break;//If not 0, then return 0
                    }

                //HALT
                case 43:
                    i = 99;
                    break;

                //INVALID OPCODE
                default:
                    cout << "ERROR: Invalid operation '" << opcode << "' at line" << i << ". "<<
                    "Please review valid instructions in readme.txt\nEnding program..." << endl;
                    i = 99;
                    break;
            }
        }
    }

}

