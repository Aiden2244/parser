// Reader.h
#ifndef READER_H
#define READER_H

#include <iostream>
#include <cstring>

struct Reader {
    static const unsigned short MAX_SIZE = 5000;
    char input[MAX_SIZE];
    unsigned int input_size;

    Reader();
    void prompt();
    void clear();
};

#endif // READER_H
