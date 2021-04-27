#ifndef TUI_H
#define TUI_H

// Text-based User Interface

#include "Parser.h"
#include "Cube.h"

class TUI
{
    public:

    TUI();

    // Read a line from the console and parse moves
    static char listen(Cube &cube, int &ctr);
};

#endif // TUI_H