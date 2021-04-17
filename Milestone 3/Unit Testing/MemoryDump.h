#ifndef TEST_MEMORYDUMP_H
#define TEST_MEMORYDUMP_H
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "VIEW.h"
#include "CPU.h"

using namespace std;

class MemDump : public VIEW{
public:
    static stringstream output;
    static void createDump(CPU cpu) {
        //Save globals
        n_result = cpu.memory;
        n_accumulator = cpu.registers[ACCUMULATOR];
        n_IC = cpu.registers[IC];
        n_opcode = cpu.registers[OPCODE];
        n_operand = cpu.registers[OPERAND];
        n_IR = cpu.IR;

        output.str(""); //reset stream
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
        output << "REGISTERS" <<
             "\nAccumulator: " << accString <<
             "\nInstruction Counter: "; output.fill('0');
             output.width(2); output << cpu.registers[IC] << "\nInstruction Register: ";
             output.fill('0'); output.width(2); output << cpu.IR << "\nOpcode: " << cpu.registers[OPCODE] << "\nOperand: ";
             output.fill('0'); output.width(2); output << cpu.registers[OPERAND] << endl << endl;

        output << "\n\n\nMEMORY\n       00     01     02     03     04     05     06     07     08     09" << endl;
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
                output << line << endl;
        }
        for (int i = 0; i < 7; i++){
            output << endl;
        }
//        output << "Press ENTER key to return back to the Main Menu > ";
//        output.flush();
    }
private:
    MemDump()= default;;
};
#endif