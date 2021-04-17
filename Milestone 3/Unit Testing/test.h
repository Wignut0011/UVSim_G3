#ifndef TEST_H
#define TEST_H
#include <cstddef>
#include <iostream>
using std::size_t;

// test.h by Chuck Allison (CC License)
// Modified by Daniel Espinel for educational purposes (2021)

namespace {
    size_t nPass{0};
    size_t nFail{0};
    void do_fail(const char* text, const char* fileName, long lineNumber){
        std::cout << "test: " << text << " in file " << fileName << " on line "
                  << lineNumber << std::endl;
        nFail++;
    }
    void do_test(const char* testText, bool cond, const char* filename, long lineNumber){
        if (cond)
            nPass++;
        else {
            std::cout << "Failed ";
            do_fail(testText, filename, lineNumber);
        }
    }
    void results_(){
        std::cout << "RESULTS\n-------" << nPass << " passed\n" << nFail << " failed" << std::endl;
    }
}
#define fail_(text)         do_fail(text, __FILE__, __LINE__)
#define test_(condition)    do_test(#condition, condition, __FILE__, __LINE__)
#define nothrow_(test)                                              \
    try{                                                            \
        test;                                                       \
        pass++;                                                     \
    }catch(...){                                                    \
        std::cout << "Failed throw ";                               \
        do_fail(#test, __FILE__, __LINE__);                         \
}

#endif