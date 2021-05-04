#ifndef SOLVER_H
#define SOLVER_H

#include <string>
#include "Cube.h"

class Solver
{

    public:

    // Executes a move sequence silently and increments ctr
    void execute(Cube &cube, std::string sequence, int &ctr, bool debug = false);
};

#endif // SOLVER_H