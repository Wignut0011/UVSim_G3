#ifndef CPU_H
#define CPU_H
#include <map> //Memory
#include <string>
#include <algorithm> //If needed
#include <iostream>
#include <utility>

//SUBCLASS CPU
class CPU {
private: VIEW& view;
public:
    //variables
    map<size_t ,string> memory; //The memory
    int accumulator; //The singular register
    int IC; //For dump
    string IR; //For dump
    size_t opcode; //Instruction code
    int operand; //Instruction operation
    string userNum; //String for user I/O prompts
    bool halt;

    CPU (VIEW& v) :view(v){accumulator = 0; IC = 0; IR = ""; opcode = 0; operand = 0; userNum = ""; halt = true;}

    //this function needs to reference the map class.
    void runCPU(map<size_t ,string> m){
        memory = move(m);
        accumulator = 0;
        halt = false;

        //For loop to imitate a cpu clock, ends when end of memory is reached or halt was set
        for (int i = 0; !halt && i < 100; i++) {
            opcode = 0;
            operand = 0;

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
                opcode = abs(stoi(line.substr(1, 2))); //Extract opcode substring
                //==========================================================
                //   Gets the data code to be used in the switch
                operand = stoi(line.substr(3)); //Extract operand substring
                // Put sign in operand
                if (line[0] == '-')
                    operand *= -1;
            }
            //switch case for each instruction
            switch (opcode) {
                case 10:
                    //Read();
                    //1007 = grab first input from the user and put it into desired memory location
                    view.DisplayRead();
                    cin >> userNum;

                    //Add sign to input if user did not
                    if (userNum[0] != '+' && userNum[0] != '-')
                        userNum.insert(0, "+");

                    memory[operand] = userNum;
                    break;

                case 11:
                    //Write();
                    //write command; take memory location 09 and give it to the screen to print.
                    view.DisplayWrite(operand, StrToInt(memory[operand]));
                    break;

                case 20:
                    //Load();    Load a word from a specific location in memory into the accumulator
                    //load command; integer from location 07 is loaded into accumulator
                    accumulator = StrToInt(memory[operand]);
                    break;

                case 21:
                    //Store();   Store a word from the accumulator into a specific location in memory
                    //store command; take the added number and store it in the memory location 09
                    memory[operand] = to_string(accumulator);

                    //Add sign to word
                    if (accumulator >= 0)
                        memory[operand].insert(0, "+");

                    break;

                case 30:
                    //Add();
                    //Extract number with sign
                    if (sign) accumulator += StrToInt(memory[operand]);
                    else accumulator -= StrToInt(memory[operand]);
                    overflowCheck();
                    break;

                case 31:
                    //Subtract();
                    //Extract number with sign
                    if (sign) accumulator -= StrToInt(memory[operand]);
                    else accumulator += StrToInt(memory[operand]);
                    overflowCheck();
                    break;

                case 32:
                    //Divide();
                    accumulator /= StrToInt(memory[operand]);
                    if (!sign) accumulator *= -1;
                    overflowCheck();
                    break;

                case 33:
                    //Multiply();
                    accumulator *= StrToInt(memory[operand]);
                    if (!sign) accumulator *= -1;
                    overflowCheck();
                    break;

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
                    //If accumulator is 0, branch to address
                    if (!accumulator)
                        i = abs(operand) - 1;
                    break;

                    //HALT
                case 43:
                    IC = i;
                    halt = true;
                    break;

                    //INVALID OPCODE
                default:
                    //stop execution and then tell view to display error.
                    halt = true;
                    IC = i;
                    //2 is going to be this error.
                    //2, line, opcode
                    view.DisplayError(2,i,opcode);

                    break;
            }
            if (!halt)
                IC = i;
        }
        view.DisplayEnd();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();

        //Format Accumulator to string
        string accString = to_string(accumulator);
        if (accumulator > 0) //Insert positive sign
            accString.insert(0, "+");
        if (accString.size() < 4) //Insert leading 0s
            for (std::size_t i = accString.size(); i < 5; ++i)
                accString.insert(1, "0");
    }

    void overflowCheck(){
        //Overflow
        //error code 3
        //DisplayError(3,0,0)
        if (accumulator > 9999){
            view.DisplayError(3,0,0);
            accumulator = -9999 + (accumulator - 9999);
        }

        //Underflow
        //DisplayError(4,0,0)
        else if (accumulator < -9999){ //Underflow
            view.DisplayError(4,0,0);
            accumulator = 9999 - (accumulator + 9999);
        }

        //Multiple overflows check
        if (accumulator > 9999 || accumulator < -9999)
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