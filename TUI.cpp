#include "TUI.h"
#include "Parser.h"
#include <iostream>
#include <string>

TUI::TUI() {}

char TUI::listen(Cube &cube, int &ctr)
{
    std::string line;
    std::cout << "Enter a (sequence of) move(s): ";
    std::getline(std::cin, line);

    if (line[0] == 'h' or line[0] == 'H')
    {
        std::cout << "Available clockwise moves are the following:\n";
        std::cout << "Rotate faces:\n\tR for right\n\tL for left\n\tU for up\n\tD for down\n\tF for front\n\tB for back\n";
        std::cout << "Rotate lines between faces:\n\tM for middle, on L\n\tE for equator, on D\n\tS for standing, on F\n";
        std::cout << "Rotate the cube:\n\tX for rotating on R\n\tY for rotating on U\n\tZ for rotating on F\n";
        std::cout << "For other rotations append to a rotation:\n\t' for anticlockwise\n\t2 for double\n";
    } else if (line[0] == 'q' or line[0] == 'Q')
    {
        std::cout << "\tReally quit? (Y/n): ";
        char ch;
        std::cin >> ch;
        if (ch == 'Y') return 'q';
    } else
        Parser::parse(cube, line, ctr);

    return 0;
}