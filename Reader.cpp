#include "Reader.h"

// construct the reader
Reader::Reader() {
    Reader::clear();
    input_size = 0;
}

// prompt the user for input
void Reader::prompt() {

    input_size = 0;
    int parenthesis_counter = 0; // keeps track of mismatched parentheses
    char* writePtr = input; // iterates through the string and processes it

    do {
        // Issue the appropriate prompt based on parenthesis_counter
        if (parenthesis_counter == 0) {
            std::cout << "-> ";
        } else {
            std::cout << "> ";
        }

        // Read the next line into a temporary buffer
        char buffer[MAX_SIZE];
        std::cin.getline(buffer, MAX_SIZE);

        // check if the last written character was a space
        bool lastWasSpace = (writePtr != input && *(writePtr - 1) == ' ');  

        // process input in place
        for (char* readPtr = buffer; *readPtr && *readPtr != ';'; ++readPtr) {
            // Update parenthesis counter and handle ')' appropriately
            if (*readPtr == '(') {
                parenthesis_counter++;
            } 
            else if (*readPtr == ')') {
                // avoid writing extra closing parenthesis
                if (parenthesis_counter <= 0) {
                    continue; 
                }
                parenthesis_counter--;
            }

            // Erase extraneous spaces
            if (*readPtr == ' ') {
                if (!lastWasSpace) {
                    *writePtr++ = ' ';
                    lastWasSpace = true;
                    input_size++;
                }
            } 
            else {
                *writePtr++ = *readPtr;
                lastWasSpace = false;
                input_size++;
            }
        }
    } while (parenthesis_counter > 0);

    // add the null terminator to the end of input
    *writePtr = '\0';
}

// Reset the input array
void Reader::clear() {
    // initialize the array to the null terminator
    std::memset(input, '\0', sizeof(input));
}



