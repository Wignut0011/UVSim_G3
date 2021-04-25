#ifndef CPU_H
#define CPU_H
#include <map> //Memory
#include <string>
#include <utility>
#include "VIEW.h"

/// Dane Manley      - Overall Design
/// Anthony Peterson - Arithmetic Instructions
///                  - "Double Word" feature
/// Santiago Ramirez - IO and LOAD/STORE Instructions
/// Daniel Espinel   - Branch Instructions

enum registerIndex: size_t{ACCUMULATOR = 0, IC = 1, OPCODE = 2, OPERAND = 3, LONG_ACC1 = 98, LONG_ACC2 = 99};

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
    bool sign;
    bool doubleWord = false; // indicates if the accumulator is currently using a double word

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
                ///Santiago Ramirez Begin
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
                                view.DisplayInvalid(INV_MESSAGE);
                                break;
                            }
                        }
                        if ((userNum.size()>6)){
                            userNum = "";
                            view.DisplayInvalid(INV_MESSAGE);
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
                    // check for double words
                    if (doubleWord)
                    {
                        view.DisplayWrite(registers[OPERAND], StrToInt(memory[registers[OPERAND]] + memory[registers[OPERAND] + 1]));
                    }
                    else view.DisplayWrite(registers[OPERAND], StrToInt(memory[registers[OPERAND]]));
                    break;

                case 20:
                    //Load();    Load a word from a specific location in memory into the accumulator
                    //load command; integer from location 07 is loaded into accumulator
                    // check for double words

                    if (memory[registers[OPERAND]][1] == '+' || memory[registers[OPERAND]][1] == '-')// double word found in memory
                    {
                        doubleWord = true;
                        memory[LONG_ACC1] = memory[registers[OPERAND]];
                        memory[LONG_ACC2] = memory[registers[OPERAND] + 1];
                    }
                    else
                    {
                        doubleWord = false;
                        registers[ACCUMULATOR] = StrToInt(memory[registers[OPERAND]]);
                        //if (memory[registers[OPERAND]][0] == '-') registers[ACCUMULATOR] *= -1;
                    }
                    break;

                case 21:
                    //Store();   Store a word from the accumulator into a specific location in memory
                    //store command; take the added number and store it in the memory location 09
                    // If accumulator is holding a 3 digit word, this indicates the accumulator is holding the first half of a double world. All other words should be 4 digits.
                    // If it is, store 98 in the specified location and 99 in the proceeding location

                    //if (to_string(registers[ACCUMULATOR]).length() == 3)
                    if (doubleWord)
                    {
                        memory[registers[OPERAND]] = memory[LONG_ACC1];
                        memory[registers[OPERAND]+1] = memory[LONG_ACC2];
                    }
                    else
                    {
                        memory[registers[OPERAND]] = to_string(registers[ACCUMULATOR]);
                        //Add sign to word
                        if (registers[ACCUMULATOR] >= 0) memory[registers[OPERAND]].insert(0, "+");
                        //else memory[registers[OPERAND]].insert(0, "-");
                    }
                    break;
                ///Santiago Ramirez End

                ///Anthony Peterson Begin
                case 30:
                    //Add();
                    initializeAccumulator();
                    // Add double word from memory to accumulator
                    if (memory[registers[OPERAND]][1] == '+' || memory[registers[OPERAND]][1] == '-')// double word found in memory
                    {
                        if (sign) registers[ACCUMULATOR] += StrToInt((memory[registers[OPERAND]]) + memory[registers[OPERAND] + 1]); // accumulator adds full 2 word number
                        else registers[ACCUMULATOR] -= StrToInt((memory[registers[OPERAND]]) + memory[registers[OPERAND] + 1]);
                        if (registers[ACCUMULATOR] <= -9999 || registers[ACCUMULATOR] >= 9999) doubleWord = true;
                        else doubleWord = false;
                    }
                    else registers[ACCUMULATOR] += StrToInt(memory[registers[OPERAND]]);
                    sixDigitSplit();
                    break;

                case 31:
                    //Subtract();
                    initializeAccumulator();
                    // Subtract double word from memory from accumulator
                    if (memory[registers[OPERAND]][1] == '+' || memory[registers[OPERAND]][1] == '-')// double word found in memory
                    {
                        if (sign) registers[ACCUMULATOR] -= StrToInt((memory[registers[OPERAND]]) + memory[registers[OPERAND] + 1]); // accumulator subtracts full 2 word number
                        else registers[ACCUMULATOR] += StrToInt((memory[registers[OPERAND]]) + memory[registers[OPERAND] + 1]);
                        if (registers[ACCUMULATOR] <= -9999 || registers[ACCUMULATOR] >= 9999) doubleWord = true;
                        else doubleWord = false;
                    }
                    else registers[ACCUMULATOR] -= StrToInt(memory[registers[OPERAND]]);
                    sixDigitSplit();
                    break;

                case 32:
                    //Divide();
                    initializeAccumulator();
                    // Divide accumulator with double word from memory
                    if (memory[registers[OPERAND]][1] == '+' || memory[registers[OPERAND]][1] == '-')// double word found in memory
                    {
                        registers[ACCUMULATOR] /= StrToInt((memory[registers[OPERAND]]) + memory[registers[OPERAND] + 1]); // accumulator subtracts full 2 word number
                        if (registers[ACCUMULATOR] <= -9999 || registers[ACCUMULATOR] >= 9999) doubleWord = true;
                        else doubleWord = false;
                    }
                    else registers[ACCUMULATOR] /= StrToInt(memory[registers[OPERAND]]);
                    sixDigitSplit();
                    break;

                case 33:
                    //Multiply();
                    initializeAccumulator();
                    // Divide accumulator with double word from memory
                    if (memory[registers[OPERAND]][1] == '+' || memory[registers[OPERAND]][1] == '-')// double word found in memory
                    {
                        registers[ACCUMULATOR] *= StrToInt((memory[registers[OPERAND]]) + memory[registers[OPERAND] + 1]); // accumulator subtracts full 2 word number
                        if (registers[ACCUMULATOR] <= -9999 || registers[ACCUMULATOR] >= 9999) doubleWord = true;
                        else doubleWord = false;
                    }
                    else registers[ACCUMULATOR] *= StrToInt(memory[registers[OPERAND]]);
                    sixDigitSplit();
                    break;
                ///Anthony Peterson End

                ///Daniel Espinel Begin
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
                ///Daniel Espinel End

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

    ///Anthony Peterson
    void initializeAccumulator()
    {
        /// Makes sure accumulator is properly initialized with double words
        if (registers[ACCUMULATOR] >= 0) sign = true;
        else if (registers[ACCUMULATOR] < 0) sign = false;
        else view.DisplayError(1,0,0);

        if (doubleWord) // double word found in accumulator
        {
            if (memory[LONG_ACC1][0] == '+') sign = true;
            else if (memory[LONG_ACC1][0] == '-') sign = false;
            else view.DisplayError(1,0,0);
            registers[ACCUMULATOR] = StrToInt(memory[LONG_ACC1] + memory[LONG_ACC2]); // accumulator is properly initialized with 2 word number
            if (!sign) registers[ACCUMULATOR] *= -1;
        }
    }
    ///Anthony Peterson
    void sixDigitSplit()
    {
        /// After the accumulator splits a word up, it automatically stores it in slots 98 and 99. Accumulator should hold the leading word afterwards (98). 98 and 99 are exclusively used for accumulator memory.
        /// ++ indicates a positive 6 digit word, -- indicates a negative 6 digit word.

        overflowCheck();
        if (((registers[ACCUMULATOR]) > 9999 && registers[ACCUMULATOR] < 999999) || (registers[ACCUMULATOR] < -9999 && registers[ACCUMULATOR] > -999999))
        {
            if (registers[ACCUMULATOR] < 0) sign = false;
            int digitTracker = 0;
            string temp = to_string(abs(registers[ACCUMULATOR]));
            stringstream s1;
            if (sign) s1 << "++";
            else s1 << "--";
            s1 << temp[digitTracker++] << temp[digitTracker++] << temp[digitTracker++];
            memory[LONG_ACC1] = s1.str(); // 98 is initialized

            stringstream s2;
            if (sign) s2 << "++";
            else s2 << "--";
            int tempLength = temp.length();
            if (tempLength > 5) tempLength = 5;
            while (digitTracker <= tempLength)
                s2 << temp[digitTracker++];
            memory[LONG_ACC2] = s2.str(); // 99 is initialized

            if (abs(StrToInt(memory[LONG_ACC2])) > 999) // 99 overflows to 4 digits
            {
                int x = StrToInt(memory[LONG_ACC2]) - 1000;
                memory[LONG_ACC2] = to_string(x);
                int y = StrToInt(memory[LONG_ACC1]) + 1;
                memory[LONG_ACC1] = to_string(y);
            }

            registers[ACCUMULATOR] = StrToInt(memory[LONG_ACC1]);// + memory[LONG_ACC2]);
            doubleWord = true;
        }
    }

    ///Anthony Peterson
    void overflowCheck(){
        //Overflow
        //error code 3
        //DisplayError(3,0,0)
        if (registers[ACCUMULATOR] > 999999){
            view.DisplayError(3,0,0);
            registers[ACCUMULATOR] = -999999 + (registers[ACCUMULATOR] - 999999);
        }

            //Underflow
            //DisplayError(4,0,0)
        else if (registers[ACCUMULATOR] < -999999){ //Underflow
            view.DisplayError(4,0,0);
            registers[ACCUMULATOR] = 999999 - (registers[ACCUMULATOR] + 999999);
        }

        //Multiple overflows check
        if (registers[ACCUMULATOR] > 999999 || registers[ACCUMULATOR] < -999999)
            overflowCheck();
    }

    /// Daniel Espinel
    // Returns signed int from string
    //*stoi() doesn't like strings that have '+' or '-'
    int StrToInt(string word){
        bool neg = false;
        stringstream ss;
        if (word[0] == '-') neg = true;
        for (int i = 0; i < word.length(); i++)
        {
            if (word[i] != '+' && word[i] != '-') ss << word[i];
        }
        //cout << "ss = " << ss.str() << endl;
        int value = stoi(ss.str());
        if (neg) return (value * -1);
        else return value;
    };

};
#endif