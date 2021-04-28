#ifndef BASIC_H
#define BASIC_H

#include "Cube.h"
#include "Parser.h"
#include <string>
#include <vector>

class Basic
{
    private:

        // used to track how far the solving process is completed
        unsigned char progress[6][3][3];

        // // Stores sequences useful to complete the first step
        // std::vector<std::string> whiteCross;
        // Stores sequences of moves that have a distinct effect
        std::vector<std::string> moveSets;

        // Used to index sequences in moveSets by name
        enum moveSetNames {
            WCO_FRU,
            WCO_FLU,
            WCO_FRU_U,
            SL_R,
            SL_L,
            YCR,
            YE_A,
            YE_O,
            YCO,
            YCOO
        };

    public:

        // Constructor, sets moveSets
        Basic();

        // Goes through the solving process
        //  Param:
        //      cube is the reference to the cube to be solved
        //      printStats will cause a breakdown of the move count by step to be
        //          printed in console
        //  Return: total move count
        int solve(Cube &cube, bool printStats = false, bool printSteps = true, int *ctrs = NULL);

        // Executes a move sequence and increments ctr
        void execute(Cube &cube, std::string sequence, int &ctr);

};

#endif // BASIC_H