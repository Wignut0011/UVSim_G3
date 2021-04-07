#ifndef CPU_H
#define CPU_H
#include <map> //Memory
#include <string>
//#include <algorithm> //If needed
//#include <iostream>
#include <utility>
#include "VIEW.h"

enum registerIndex: size_t{ACCUMULATOR = 0, IC = 1, OPCODE = 2, OPERAND = 3};

//SUBCLASS CPU
class CPU {
private: VIEW& view;
public:
    //variables
    map<size_t ,string> memory; //The memory
    int registers[5];
//    int accumulator; //The singular register
//    int IC; //For dump
    string IR; //For dump
//    size_t opcode; //Instruction code
//    int operand; //Instruction operation
    string userNum; //String for user I/O prompts
    bool halt;

    CPU (VIEW& v) :view(v){registers[ACCUMULATOR] = 0; registers[IC] = 0; IR = ""; registers[OPCODE] = 0; registers[OPERAND] = 0; userNum = ""; halt = true;}

    //this function needs to reference the map class.
    void runCPU(map<size_t ,string> m){
        memory = move(m);
        registers[ACCUMULATOR] = 0;
        halt = false;

        if (memory.size()<100) //Make sure the memory is filled up
            for (size_t i = 0; i < 100; ++i) {
                if (memory[i] == "")
                    memory[i] = "+0000";
            }

        //For loop to imitate a cpu clock, ends when end of memory is reached or halt was set
        for (int i = 0; !halt && i < 100; i++) {
            registers[OPCODE] = 0;
            registers[OPERAND] = 0;

            string line = memory.at(i);
            IR = line;
            //stop gathering if user types -99999
            if (line == "-99999") break;

            // grabs Integer sign
            bool sign;
            if (line[0] == '+') sign = true;
            else if (line[0] == '-') sign = false;
            else
            {
                halt = true;
                //1 which type of error  1 is this error
                //1, line, 0
                view.DisplayError(1,i,0);
                break;
            }

            //Check if memory location is not properly formatted
            if (line.size() < 5){
                size_t diff = 5 - line.size();
                for (size_t j = 0; j < diff; j++)
                    line.insert(1, "0");
            }
            else {
                registers[OPCODE] = abs(stoi(line.substr(1, 2))); //Extract opcode substring
                //==========================================================
                //   Gets the data code to be used in the switch
                registers[OPERAND] = stoi(line.substr(3)); //Extract operand substring
                // Put sign in operand
                if (line[0] == '-')
                    registers[OPERAND] *= -1;
            }
            //switch case for each instruction
            switch (registers[OPCODE]) {
                case 10:
                    //Read();
                    //1007 = grab first input from the user and put it into desired memory location
                    view.DisplayRead("");

                    userNum = "";
                    //Make sure user inputs a number
                    while (userNum.empty()) {
                        cin >> userNum;
                        for (auto j :userNum) {
                            if (j != '-' && j != '+' && !isdigit(j)) {
                                userNum = "";
                                view.DisplayInvalid();
                                break;
                            }
                        }
                        if ((userNum.size()>6)){
                            userNum = "";
                            view.DisplayInvalid();
                        }
                    }

                    //Add sign to input if user did not
                    if (userNum[0] != '+' && userNum[0] != '-')
                        userNum.insert(0, "+");

                    //Display what the user had input
                    view.DisplayRead(userNum);

                    memory[registers[OPERAND]] = userNum;
                    break;

                case 11:
                    //Write();
                    //write command; take memory location 09 and give it to the screen to print.
                    view.DisplayWrite(registers[OPERAND], StrToInt(memory[registers[OPERAND]]));
                    break;

                case 20:
                    //Load();    Load a word from a specific location in memory into the accumulator
                    //load command; integer from location 07 is loaded into accumulator
                    registers[ACCUMULATOR] = StrToInt(memory[registers[OPERAND]]);
                    break;

                case 21:
                    //Store();   Store a word from the accumulator into a specific location in memory
                    //store command; take the added number and store it in the memory location 09
                    memory[registers[OPERAND]] = to_string(registers[ACCUMULATOR]);

                    //Add sign to word
                    if (registers[ACCUMULATOR] >= 0)
                        memory[registers[OPERAND]].insert(0, "+");

                    break;

                case 30:
                    //Add();
                    //Extract number with sign
                    if (sign) registers[ACCUMULATOR] += StrToInt(memory[registers[OPERAND]]);
                    else registers[ACCUMULATOR] -= StrToInt(memory[registers[OPERAND]]);
                    overflowCheck();
                    break;

                case 31:
                    //Subtract();
                    //Extract number with sign
                    if (sign) registers[ACCUMULATOR] -= StrToInt(memory[registers[OPERAND]]);
                    else registers[ACCUMULATOR] += StrToInt(memory[registers[OPERAND]]);
                    overflowCheck();
                    break;

                case 32:
                    //Divide();
                    registers[ACCUMULATOR] /= StrToInt(memory[registers[OPERAND]]);
                    if (!sign) registers[ACCUMULATOR] *= -1;
                    overflowCheck();
                    break;

                case 33:
                    //Multiply();
                    registers[ACCUMULATOR] *= StrToInt(memory[registers[OPERAND]]);
                    if (!sign) registers[ACCUMULATOR] *= -1;
                    overflowCheck();
                    break;

                    //BRANCH
                case 40:
                    //Branch to address in operand
                    i = abs(registers[OPERAND]) - 1;
                    break;

                    //BRANCHNEG
                case 41:
                    if (registers[ACCUMULATOR] < 0) //If accumulator is negative, branch to address
                        i = abs(registers[OPERAND]) - 1;
                    break;

                    //BRANCHZERO
                case 42:
                    //If registers[ACCUMULATOR] is 0, branch to address
                    if (!registers[ACCUMULATOR])
                        i = abs(registers[OPERAND]) - 1;
                    break;

                    //HALT
                case 43:
                    registers[IC] = i;
                    halt = true;
                    break;

                    //INVALID OPCODE
                default:
                    //stop execution and then tell view to display error.
                    halt = true;
                    registers[IC] = i;
                    //2 is going to be this error.
                    //2, line, opcode
                    view.DisplayError(2,i,registers[OPCODE]);

                    break;
            }
            if (!halt)
                registers[IC] = i;
        }
        view.DisplayEnd();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        //Format Accumulator to string
        string accString = to_string(registers[ACCUMULATOR]);
        if (registers[ACCUMULATOR] > 0) //Insert positive sign
            accString.insert(0, "+");
        if (accString.size() < 4) //Insert leading 0s
            for (std::size_t i = accString.size(); i < 5; ++i)
                accString.insert(1, "0");
    }

    void overflowCheck(){
        //Overflow
        //error code 3
        //DisplayError(3,0,0)
        if (registers[ACCUMULATOR] > 9999){
            view.DisplayError(3,0,0);
            registers[ACCUMULATOR] = -9999 + (registers[ACCUMULATOR] - 9999);
        }

        //Underflow
        //DisplayError(4,0,0)
        else if (registers[ACCUMULATOR] < -9999){ //Underflow
            view.DisplayError(4,0,0);
            registers[ACCUMULATOR] = 9999 - (registers[ACCUMULATOR] + 9999);
        }

        //Multiple overflows check
        if (registers[ACCUMULATOR] > 9999 || registers[ACCUMULATOR] < -9999)
            overflowCheck();
    }

    // Returns signed int from string
    //*stoi() doesn't like strings that have '+' or '-'
    int StrToInt(string word){
        int y = stoi(word.substr(1));
        if (word[0] == '-') //Grab number sign
            return(y * -1);
        return y;
    };

};
#endif