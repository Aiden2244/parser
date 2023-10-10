#include "SymbolTable.h"

// Add a symbol to the table and return its location.
// If it already exists, return the existing location.
int SymbolTable::addSymbol(const std::string& symbol) {
    SymbolNode* current = head_;
    while (current != nullptr) {
        if (current->symbol == symbol) {
            // Symbol already exists, return its location.
            return current->location;
        }
        current = current->next;
    }

    // Symbol does not exist, add it to the table.
    SymbolNode* new_node = new SymbolNode;
    new_node->symbol = symbol;
    new_node->location = next_location_;
    new_node->next = head_;  // Insert at the beginning of the list for O(1) insertion.
    head_ = new_node;

    // Increment the next available location.
    next_location_++;

    return new_node->location;
}

// Retrieve the symbol using its location.
std::string SymbolTable::getSymbol(int location) const {
    SymbolNode* current = head_;
    while (current != nullptr) {
        if (current->location == location) {
            return current->symbol;
        }
        current = current->next;
    }
    // Symbol not found, return an empty string or handle as per your use case.
    return "";
}

// Check if the symbol exists and return its location.
// Return -1 if it does not exist.
int SymbolTable::getSymbolLocation(const std::string& symbol) const {
    SymbolNode* current = head_;
    while (current != nullptr) {
        if (current->symbol == symbol) {
            return current->location;
        }
        current = current->next;
    }
    // Symbol not found.
    return -1;
}

// Destructor to deallocate memory used by the linked list.
SymbolTable::~SymbolTable() {
    SymbolNode* current = head_;
    while (current != nullptr) {
        SymbolNode* to_delete = current;
        current = current->next;
        delete to_delete;
    }
}
