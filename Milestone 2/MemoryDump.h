#ifndef MEMORYDUMP_H
#define MEMORYDUMP_H
#include <iostream>
#include <string>
#include <map>
#include "VIEW.h"
#include "CPU.h"

using namespace std;

class MemDump {
//    static map <size_t, string> prevMap;
//    static map <size_t, string> curMap;
public:
    static void createDump(CPU cpu) {
//        curMap = cpu.memory;
//        prevMap = curMap;
        string accString = to_string(cpu.accumulator);
        if (cpu.accumulator > 0) //Insert positive sign
            accString.insert(0, "+");
        if (accString.size() < 4) //Insert leading 0s
            for (std::size_t i = accString.size(); i < 5; ++i)
                accString.insert(1, "0");

        //Print register dump
        cout << "REGISTERS" <<
             "\nAccumulator: " << accString <<
             "\nInstruction Counter: "; cout.fill('0');
             cout.width(2); cout << cpu.IC << "\nInstruction Register: ";
             cout.fill('0'); cout.width(2); cout << cpu.IR << "\nOpcode: " << cpu.opcode << "\nOperand: ";
             cout.fill('0'); cout.width(2); cout << cpu.operand << endl << endl;

        cout << "MEMORY\n       00     01     02     03     04     05     06     07     08     09" << endl;
        string line;
        for (int i = 0; i < 100; i++) {
            if (i % 10 == 0) {
                if (i == 0)
                    line = "00";

                else
                    line = to_string(i);
            }
            string memory = cpu.memory[i];
            size_t addZero = 5 - memory.length();

            for (int j = 0; j < addZero; j++)
                memory.insert(1, "0");

            line += "  " + memory;
            if (i % 10 == 9)
                cout << line << endl;
        }
        for (int i = 0; i < 19; i++){
            cout << endl;
        }
    }
private:
    MemDump()= default;;
};
#endif