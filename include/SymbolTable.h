#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>

// A simple structure to hold symbol and its location.
struct SymbolNode {
    std::string symbol;
    int location;
    SymbolNode* next;  // Pointer to the next node in the linked list.
};

class SymbolTable {
public:
    // Add a symbol to the table and return its location.
    // If it already exists, return the existing location.
    int addSymbol(const std::string& symbol);

    // Retrieve the symbol using its location.
    std::string getSymbol(int location) const;

    // Check if the symbol exists and return its location.
    // Return -1 if it does not exist.
    int getSymbolLocation(const std::string& symbol) const;

    // Destructor to deallocate memory used by the linked list.
    ~SymbolTable();

private:
    SymbolNode* head_ = nullptr;  // Head of the linked list.
    int next_location_ = 0;  // Next available location.
};

#endif // SYMBOL_TABLE_H