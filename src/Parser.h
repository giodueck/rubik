#ifndef MOVES_H
#define MOVES_H

#include <string>
#include "Cube.h"

class Parser
{
    public:

    Parser();

    // Executes the first move described in string move if valid,
    // else throws an exception
    static void move(Cube &cube, std::string str, int &ctr);

    // Executes the given move if valid, does nothing if invalid
    static void move(Cube &cube, char mov, char mod, int &ctr);

    // Tokenize cstring and execute moves. str has to be tokenizable by ' '.
    // If the cube should not be printed after each move set print accordingly
    static void parse(Cube &cube, char* str, int &ctr, bool print = false, bool printLast = true);

    // Tokenize string and execute moves. str has to be tokenizable by ' '.
    // If the cube should not be printed after each move set print accordingly
    static void parse(Cube &cube, std::string str, int &ctr, bool print = false, bool printLast = true);

};

#endif // MOVES_H