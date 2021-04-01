#include "CONTROLLER.h"
#include <stdexcept>
#include <iostream>

//using namespace std;

int main(){
    VIEW view;
    MODEL model(view);
    CONTROLLER controller((MODEL&)model, (VIEW&)view);

    try{controller.StartSimulator();}
    catch (std::logic_error &err) {
        std::cout << "\nLOGIC ERROR: " << err.what() << "\nEXITING...\n";
    }
    catch (std::runtime_error &err){
        std::cout << "\nFATAL ERROR: " << err.what() << "\nEXITING...\n";
    }
    return 0;
}

