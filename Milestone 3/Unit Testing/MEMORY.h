#ifndef MEMORY_H
#define MEMORY_H
#include <string>
#include <map>
using namespace std;
class MEMORY{
public:
    //initialize map
    map<size_t, string> inputMap;

    void add(string uInput, int memLocation) {
        inputMap[memLocation] = uInput;
    };

    map<size_t, string>& getMap(){
        return (map<size_t, string>&)inputMap;
    }

};
#endif
