#ifndef CFOP_H
#define CFOP_H

#include "Solver.h"

// create a base class with all the steps as functions
// create a subclass for simplified version

class CFOP: public Solver
{
    protected:

    public:

        // Constructor
        CFOP();

        // Goes through the solving process
        //  Param:
        //      cube is the reference to the cube to be solved
        //      printStats will cause a breakdown of the move count by step to be
        //          printed in console
        //  Return: total move count
        virtual int solve(Cube &cube, bool printStats = false, bool printSteps = true, int *ctrs = NULL, bool dbg = false) = 0;

        // Cross step
        void C(Cube &cube, int &ctr, bool dbg = false);

        // First two layers
        void F(Cube &cube, int &ctr, bool dbg = false);

        // Orient last layer
        virtual void O(Cube &cube, int &ctr, bool dbg = false) = 0;

        // Permute last layer
        virtual void P(Cube &cube, int &ctr, bool dbg = false) = 0;
};

class S_CFOP: public CFOP
{
    protected:

    public:

        // Constructor
        S_CFOP();

        // Goes through the solving process
        //  Param:
        //      cube is the reference to the cube to be solved
        //      printStats will cause a breakdown of the move count by step to be
        //          printed in console
        //  Return: total move count
        virtual int solve(Cube &cube, bool printStats = false, bool printSteps = true, int *ctrs = NULL, bool dbg = false) = 0;

        // Orient last layer
        virtual void O(Cube &cube, int &ctr, bool dbg = false) = 0;

        // Permute last layer
        virtual void P(Cube &cube, int &ctr, bool dbg = false) = 0;
};

#endif // CFOP_H