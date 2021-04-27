/* Rubik's Cube solver by Giovanni Dueck
   Version 1.0 'Manual'
   27/04/2021
*/

#include <iostream>
#include "Parser.h"
#include <vector>
#include <cstring>
#include "Cube.h"
#include "TUI.h"

int main()
{
    std::cout << "# Rubik's Cube solver #\n  For help enter 'h' or 'H'\n  To quit enter 'q' or 'Q'\n";
    
    Cube cube = Cube();
    cube.draw();
    std::cout << '\n';
    cube.scramble(20, true);
    int moveCtr = 0;

    while (!cube.isSolved())
    {
        char ch = TUI::listen(cube, moveCtr);
        if (ch == 'q') break;
    }

    if (cube.isSolved()) std::cout << "Nice! Cube solved in " << moveCtr << " moves.\n";

    return 0;
}