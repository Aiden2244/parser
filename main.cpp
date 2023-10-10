#include "Reader.h"

int main() {
    
    Reader r;

    while(strcmp(r.input, "quit") != 0){
        r.clear();
        r.prompt();
        std::cout << r.input << std::endl;
        // std::cout << "Input size: " << r.input_size << std::endl;
    }

    return 0;
}