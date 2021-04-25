#ifndef TEST_MEMORYDUMP_H
#define TEST_MEMORYDUMP_H
#include <iostream>
#include <string>
#include <map>
#include "VIEW.h"
#include "CPU.h"

using namespace std;

class MemDump : public VIEW{
public:
    static void createDump(CPU cpu) {
        //Dane Manley

        n_accumulator = cpu.registers[ACCUMULATOR];
        n_opcode = cpu.registers[OPCODE];
        n_operand = cpu.registers[OPERAND];
        n_IC = cpu.registers[IC];
        n_IR = cpu.IR;
        n_result = cpu.memory;
    }
private:
    MemDump()= default;;
};
#endif