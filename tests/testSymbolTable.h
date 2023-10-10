#ifndef TEST_SYMBOL_TABLE_H
#define TEST_SYMBOL_TABLE_H

#include "../include/SymbolTable.h"

#include <iostream>
#include <string>

/* Functions to test the SymbolTable */

// Function to test adding and retrieving symbols
bool testAddAndGetSymbol(SymbolTable& table) {
    std::string symbol = "x";
    int location = table.addSymbol(symbol);
    return table.getSymbol(location) == symbol;
}

// Function to test retrieving a symbol's location
bool testGetSymbolLocation(SymbolTable& table) {
    std::string symbol = "y";
    int location = table.addSymbol(symbol);
    return table.getSymbolLocation(symbol) == location;
}

// Function to test retrieving a non-existent symbol
bool testGetNonExistentSymbol(SymbolTable& table) {
    return table.getSymbol(999) == "";
}

// Function to test retrieving the location of a non-existent symbol
bool testGetNonExistentSymbolLocation(SymbolTable& table) {
    return table.getSymbolLocation("non_existent") == -1;
}


/* Function to run the tests above*/
void testSymbolTable() {
    std::cout << "TESTING SYMBOL TABLE\n";
    SymbolTable table;

    std::cout << "Test add and get symbol: "
              << (testAddAndGetSymbol(table) ? "PASSED" : "FAILED") << std::endl;

    std::cout << "Test get symbol location: "
              << (testGetSymbolLocation(table) ? "PASSED" : "FAILED") << std::endl;

    std::cout << "Test get non-existent symbol: "
              << (testGetNonExistentSymbol(table) ? "PASSED" : "FAILED") << std::endl;

    std::cout << "Test get non-existent symbol location: "
              << (testGetNonExistentSymbolLocation(table) ? "PASSED" : "FAILED") << std::endl;
    std::cout << '\n';
}

#endif // TEST_SYMBOL_TABLE_H