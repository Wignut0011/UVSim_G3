#ifndef MEMORY_H
#define MEMORY_H
#include <map>
#include <string>

///Santiago Ramirez

class MEMORY{
public:
    //initialize map
    std::map<size_t, std::string> inputMap;

    void add(std::string uInput, int memLocation) {
        inputMap[memLocation] = uInput;
    };

    std::map<size_t, std::string>& getMap(){
        return (std::map<size_t, std::string>&)inputMap;
    }

};
#endif
