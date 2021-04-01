#ifndef MODEL_H
#define MODEL_H
#include <utility>

#include "MemoryDump.h"
#include "CPU.h"
#include "VIEW.h"
#include "MEMORY.h"

class MODEL{
    MEMORY memory;
    VIEW& view;
    CPU cpu = CPU(view); //view is just a reference. Once the constructor reassigns view, it will cascade.

public:
    //MemDump& memDump;

    MODEL(VIEW& v) : view(v){}

    //variables
    string userNum;
    int currentPage;

    //functions
    bool hasMemory(){
        if(!memory.inputMap.empty()){
            return true;
        }
        return false;
    }

    map<size_t, string> GetMemory(){return memory.getMap();}

    ///TODO
    void runCPU(){
//        CPU cpu(view);
        cpu.runCPU(memory.getMap());
        MemDump::createDump(cpu);
    }

    ///not totally sure what this is supposed to accomplish yet. I think I got it though.
    void updateMenu(int page){
        if(page == 6 && hasMemory()){
            view.ContinueEdit(memory);
        }
        else
            view.Display(page);
    };

    //void loadMemory(MEMORY);  ///this function is probably not needed.

    void updateMemory(string uInput, int memLocation){
        ///this is going to call on memory.h to make things add to memory
        ///memlocation is going to be from 0-99
        memory.add(move(uInput), memLocation);
    }
};
#endif