#ifndef MEMORYDUMP_H
#define MEMORYDUMP_H
#include <iostream>
#include <string>
#include <map>
#include "VIEW.h"
#include "CPU.h"

using namespace std;

class MemDump : public VIEW{
public:
    static void createDump(CPU cpu) {
//        curMap = cpu.memory;
//        prevMap = curMap;
        //An empty memory, for some strange reason, made it's way here
        if (cpu.memory.empty())
            throw logic_error("For some reason, dump received an empty CPU. How? How did this happen?");

        string accString = to_string(cpu.registers[ACCUMULATOR]);
        if (cpu.registers[ACCUMULATOR] > 0) //Insert positive sign
            accString.insert(0, "+");
        if (accString.size() < 4) //Insert leading 0s
            for (std::size_t i = accString.size(); i < 5; ++i)
                accString.insert(1, "0");

        //Print register dump
        cout << "REGISTERS" <<
             "\nAccumulator: " << accString <<
             "\nInstruction Counter: "; cout.fill('0');
             cout.width(2); cout << cpu.registers[IC] << "\nInstruction Register: ";
             cout.fill('0'); cout.width(2); cout << cpu.IR << "\nOpcode: " << cpu.registers[OPCODE] << "\nOperand: ";
             cout.fill('0'); cout.width(2); cout << cpu.registers[OPERAND] << endl << endl;

        cout << "\n\n\nMEMORY\n       00     01     02     03     04     05     06     07     08     09" << endl;
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
        for (int i = 0; i < 7; i++){
            cout << endl;
        }
        cout << "Press ENTER key to return back to the Main Menu > ";
        cout.flush();
    }
private:
    MemDump()= default;;
};
#endif