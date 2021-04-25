#ifndef MODEL_H
#define MODEL_H
#include <utility>

#include "MemoryDump.h"
#include "CPU.h"
#include "VIEW.h"
#include "MEMORY.h"

///Santiago Ramirez

class MODEL{
    MEMORY memory;
    VIEW& view;
    CPU cpu = CPU(view); //view is just a reference. Once the constructor reassigns view, it will cascade.

public:

    MODEL(VIEW& v) : view(v){}

    //functions
    bool hasMemory(){
        if(!memory.inputMap[0].empty()){
            return true;
        }
        return false;
    }

    MEMORY& GetMemory(){return (MEMORY &)memory;}

    void runCPU(){
//        CPU cpu(view);
        cpu.runCPU(memory.getMap());
        MemDump::createDump(cpu);//Send results to dump
        cin.get();
    }

    //not totally sure what this is supposed to accomplish yet. I think I got it.
    void updateMenu(int page){
        if(page == mainPages::EDIT && hasMemory()){
            view.ContinueEdit(memory);
        }
        else
            view.Display(page);
    };

    void loadMemory(MEMORY mem){
        memory = move(mem);
    }

    void updateMemory(string uInput, int memLocation){
        //this is going to call on memory.h to make things add to memory
        //memlocation is going to be from 0-99
        memory.add(move(uInput), memLocation);
    }
};
#endif