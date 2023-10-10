#include "include/Reader.h"

// Testing suite
#ifdef RUN_TESTS
#include "tests/testSuite.h"

void runAllTests() {
    testSymbolTable();
}
#endif // RUN_TESTS

int main() {

    #ifdef RUN_TESTS
        runAllTests();
    #endif // RUN_TESTS
    
    Reader r;

    while(strcmp(r.input, "quit") != 0){
        r.clear();
        r.prompt();
        std::cout << r.input << std::endl;
        // std::cout << "Input size: " << r.input_size << std::endl;
    }

    return 0;
}