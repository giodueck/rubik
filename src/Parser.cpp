#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "Parser.h"
#include "Cube.h"

void testprint(char mov, char mod)
{
    std::cout << mov;
    if (mod == 1) std::cout << '\'';
    else if (mod == 2) std::cout << '2';
    std::cout << ' ';
}

void Parser::move(Cube &cube, char mov, char mod, int &ctr)
{
    switch (mov)
    {
        case 'R':
            cube.R(mod);
            ctr++;
            break;
        case 'L':
            cube.L(mod);
            ctr++;
            break;
        case 'U':
            cube.U(mod);
            ctr++;
            break;
        case 'D':
            cube.D(mod);
            ctr++;
            break;
        case 'F':
            cube.F(mod);
            ctr++;
            break;
        case 'B':
            cube.B(mod);
            ctr++;
            break;
        case 'M':
            cube.M(mod);
            ctr++;
            break;
        case 'E':
            cube.E(mod);
            ctr++;
            break;
        case 'S':
            cube.S(mod);
            ctr++;
            break;
        case 'X':
            cube.X(mod);
            break;
        case 'Y':
            cube.Y(mod);
            break;
        case 'Z':
            cube.Z(mod);
            break;
    }
}

void Parser::move(Cube &cube, std::string str, int &ctr)
{
    // Interprets a move from a string and calls the other move
    char mov = 0;
    char mod = 0;

    switch (str[0])
    {
        case 'R': case 'L': case 'U': case 'D': case 'F': case 'B':
        case 'M': case 'E': case 'S':
        case 'X': case 'Y': case 'Z':
            mov = str[0];
            break;
        default:
            throw "Invalid move";
    }

    switch (str[1])
    {
        case '\'':
            mod = 1;
            break;
        case '2':
            mod = 2;
            break;
        case ' ': case '\0': case '\t': case '\n':
            mod = 0;
            break;
        default:
            throw "Invalid modifier";
    }

    Parser::move(cube, mov, mod, ctr);
}

void Parser::parse(Cube &cube, char* str, int &ctr, bool print, bool printLast)
{
    // Tokenizes str and evaluates each resulting move substring
    std::vector<char*> moves;
    char* aux;
    const char* delim = " \n";

    aux = strtok(str, " \n");
    if (aux) moves.push_back(aux);

    while (aux)
    {
        aux = strtok(NULL, " \n");
        if (aux) moves.push_back(aux);
    }

    for (int i = 0; i < moves.size(); i++)
    {
        Parser::move(cube, moves[i], ctr);
        if (print or printLast and i == (moves.size() - 1))
        {
            cube.draw();
            std::cout << '\n';
        }
    }
}

void Parser::parse(Cube &cube, std::string str, int &ctr, bool print, bool printLast)
{
    // just converts string to cstring and runs the other parse function
    int len = str.length();
    char cstr[len + 1];

    for (int i = 0; i < len; i++)
    {
        cstr[i] = str[i];
    }

    cstr[len] = '\0';

    Parser::parse(cube, cstr, ctr, print, printLast);
}

