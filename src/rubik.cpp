/* Rubik's Cube solver by Giovanni Dueck
   Version 0.2.x 'Basic' WIP
   27/04/2021
*/

#include <iostream>
#include "Parser.h"
#include <vector>
#include <cstring>
#include "Cube.h"
#include "TUI.h"
#include "Algorithms.h"

int main()
{
    std::string line;
    const char* cline;
    std::cout << "# Rubik's Cube solver #\n";

    while (true)
    {
        std::cout << "  Select algorithm:\n\t0 - Manual\n\t1 - Beginner\n  > ";
        std::cin >> line;
        cline = line.c_str();
        if (cline[0] >= '0' and cline[0] <= '1')
        {
            // create and scramble cube
            Cube cube = Cube();
            int dbg = 0;
            // Parser::parse(cube, (std::string)"L L2 M2 D M M2 E2 D' R2 R2 R U2 U2 D2 U2 F E U U' U2", dbg, false);
            cube.scramble(20, dbg);

            if (cline[0] == '0')
            {
                std::cout << "  For help enter 'h' or 'H'\n  To quit enter 'q' or 'Q'\n";
                cube.draw();
                std::cout << '\n';
                int moveCtr = 0;

                while (!cube.isSolved())
                {
                    char ch = TUI::listen(cube, moveCtr);
                    if (ch == 'q') break;
                }

                if (cube.isSolved()) std::cout << "Nice! Cube solved in " << moveCtr << " moves.\n";

                // goto is good for exiting loops inconditionally
                goto END;
            } else if (cline[0] == '1')
            {
                std::cout << "Print stats at the end? (Y/n) ";
                char ch;
                bool stats = false;
                std::cin >> ch;
                if (ch == 'Y') stats = true;

                std::cout << "Print cube at the end of key steps? (Y/n) ";
                bool steps = false;
                std::cin >> ch;
                if (ch == 'Y') steps = true;

                Basic solver = Basic();
                std::cout << "Start:\n";
                cube.draw();
                std::cout << '\n';
                solver.solve(cube, stats, steps);
                goto END;
            }
        }
    }

    END:
    return 0;
}