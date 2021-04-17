#include "CONTROLLER.h"
#include <stdexcept>
#include <iostream>
#include <string>

//using namespace std;

string clipboard; //Clipboard to be used in the program

int main(){
    VIEW view(string& clipboard);
    MODEL model((VIEW&)view);
    CONTROLLER controller((MODEL&)model, (VIEW&)view, (string&) clipboard);

    try{controller.StartSimulator();}
    catch (std::logic_error &err) {
        std::cout << "\nLOGIC ERROR: " << err.what() << "\nEXITING...\n";
    }
    catch (std::runtime_error &err){
        std::cout << "\nFATAL ERROR: " << err.what() << "\nEXITING...\n";
    }
    return 1; //Exit happens in CONTROLLER. This exit was abnormal
}

