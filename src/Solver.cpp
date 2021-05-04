#include "Solver.h"
#include "Parser.h"
#include <iostream>

void Solver::execute(Cube &cube, std::string sequence, int &ctr, bool debug)
{
    if (debug) std::cout << "\tExecute " << sequence << '\n';
    Parser::parse(cube, sequence, ctr, false, debug);
}