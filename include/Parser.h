#ifndef PARSER_H
#define PARSER_H

#include "SymbolTable.h"
#include <string>

// A simple structure to hold a list of symbol locations.
struct NameListNode {
    int location;
    NameListNode* next;  // Pointer to the next node in the linked list.
};

class Parser {
public:
    Parser(SymbolTable& symbol_table);

    // Parse a name and add it to the symbol table.
    int parseName(const std::string& input, int& pos);

    // Parse a list of names and return a linked list of their locations in the symbol table.
    NameListNode* parseNameList(const std::string& input, int& pos);

    // Parse a function definition.
    int parseDef(const std::string& input, int& pos);

    // Add a new function definition to the function definitions list.
    void newFunDef(int name_location, NameListNode* arglist);

    // Additional parsing functions can be added here as per the requirements in DESIGN.pdf.

private:
    SymbolTable& symbol_table_;
    // Additional private member variables and helper functions can be added as needed.
};


#endif // PARSER_H