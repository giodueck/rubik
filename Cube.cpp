#include "Cube.h"
#include "Face.h"
#include "Parser.h"
#include <time.h>
#include <random>
#include <vector>
#include <iostream>
#include <windows.h>

// Constructors

Cube::Cube()
{
    unsigned char colors[6] = { 32, 16, 224, 112, 64, 96 };
    for (int i = 0; i < 6; i++)
    {
        faces[i] = Face(colors[i]);
    }
}

Cube::Cube(char* colors)
{
    for (int i = 0; i < 6; i++)
    {
        faces[i] = Face(colors[i]);
    }
}

Cube::Cube(Face faces[6])
{
    for (int i = 0; i < 6; i++)
    {
        this->faces[i] = faces[i];
    }
}

// Functions

bool Cube::isSolved()
{
    for (int i = 0; i < 6; i++)
    {
        if (!faces[i].isSolved()) return false;
    }
    return true;
}

void Cube::scramble(int numMoves, bool print)
{
    // set seed to current time
    std::srand(time(0));

    char moveSet[10] = "RLUDFBMES";

    // Create a move string
    char moves[numMoves * 3 + 1];
    int i, j;
    for (i = 0, j = 0; j < numMoves; i++, j++)
    {
        moves[i] = moveSet[std::rand() % 9];
        i++;
        moves[i] = std::rand() % 3;
        
        if (moves[i] == 1)
        {
            moves[i] = '\'';
            i++;
        } else if (moves[i] == 2)
        {
            moves[i] = '2';
            i++;
        }

        moves[i] = ' ';
    }
    moves[i] = '\0';

    if (print) std::cout << "Scrambling sequence:" << moves << '\n';
    int dud = 0;
    Parser::parse(*this, moves, dud, false);
}

void Cube::draw()
{
    // Prints spaces in console with the appropriate background color
    // The layout is
    //   U
    // L F R B
    //   D

    HANDLE  hConsole;
    int c;
    unsigned char aux[3][3];

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Up
    faces[2].copy(aux);
    for (int i = 0; i < 3; i++)
    {
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "      ";
        for (int j = 0; j < 3; j++)
        {
            SetConsoleTextAttribute(hConsole, aux[i][j]);
            std::cout << "  ";
        }
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << '\n';
    }

    for (int i = 0; i < 3; i++)
    {
        // Left
        faces[1].copyRow(i, aux[0]);
        for (int j = 0; j < 3; j++)
        {
            SetConsoleTextAttribute(hConsole, aux[0][j]);
            std::cout << "  ";
        }

        // Front
        faces[4].copyRow(i, aux[0]);
        for (int j = 0; j < 3; j++)
        {
            SetConsoleTextAttribute(hConsole, aux[0][j]);
            std::cout << "  ";
        }

        // Right
        faces[0].copyRow(i, aux[0]);
        for (int j = 0; j < 3; j++)
        {
            SetConsoleTextAttribute(hConsole, aux[0][j]);
            std::cout << "  ";
        }

        // Back
        faces[5].copyRow(i, aux[0]);
        for (int j = 0; j < 3; j++)
        {
            SetConsoleTextAttribute(hConsole, aux[0][j]);
            std::cout << "  ";
        }

        // next row
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << '\n';
    }

    // Down
    faces[3].copy(aux);
    for (int i = 0; i < 3; i++)
    {
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "      ";
        for (int j = 0; j < 3; j++)
        {
            SetConsoleTextAttribute(hConsole, aux[i][j]);
            std::cout << "  ";
        }
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << '\n';
    }
    
    // just to be safe
    SetConsoleTextAttribute(hConsole, 7);
}

// Moves
/*
    Orientations of faces: 
    Front is reference
    Left and Right and Back have the same orientation
    Down has up direction pointing to Front
    Up has up direction pointing away from Front 
*/

void Cube::R(char mod)
{
    unsigned char aux[3];
    unsigned char aux2[3];
    if (!mod)
    {
        // Rotate face
        faces[Facing::Right].rotate();
        // Rotate lines the touch the face as well
        // store original front col
        faces[Facing::Front].copyCol(2, aux);
        // copy down->front
        faces[Facing::Down].copyCol(2, aux2);
        faces[Facing::Front].replaceCol(2, aux2);
        // copy back->down
        faces[Facing::Back].copyCol(0, aux2, true);
        faces[Facing::Down].replaceCol(2, aux2);
        // copy up->back
        faces[Facing::Up].copyCol(2, aux2, true);
        faces[Facing::Back].replaceCol(0, aux2);
        // copy stored front->up
        faces[Facing::Up].replaceCol(2, aux);
    } else if (mod == 1)
    {
        // Rotate face
        faces[Facing::Right].rotate(mod);
        // Rotate lines the touch the face as well
        // store original front col
        faces[Facing::Front].copyCol(2, aux);
        // copy up->front
        faces[Facing::Up].copyCol(2, aux2);
        faces[Facing::Front].replaceCol(2, aux2);
        // copy back->up
        faces[Facing::Back].copyCol(0, aux2, true);
        faces[Facing::Up].replaceCol(2, aux2);
        // copy down->back
        faces[Facing::Down].copyCol(2, aux2, true);
        faces[Facing::Back].replaceCol(0, aux2);
        // copy stored front->down
        faces[Facing::Down].replaceCol(2, aux);
    } else if (mod == 2)
    {
        // Rotate face
        faces[Facing::Right].rotate(mod);
        // Rotate lines the touch the face as well
        // swap front<->back
        faces[Facing::Front].copyCol(2, aux, true);
        faces[Facing::Back].copyCol(0, aux2, true);
        faces[Facing::Front].replaceCol(2, aux2);
        faces[Facing::Back].replaceCol(0, aux);
        // swap up<->down
        faces[Facing::Up].copyCol(2, aux);
        faces[Facing::Down].copyCol(2, aux2);
        faces[Facing::Up].replaceCol(2, aux2);
        faces[Facing::Down].replaceCol(2, aux);
    }
}

void Cube::L(char mod)
{
    unsigned char aux[3];
    unsigned char aux2[3];
    if (!mod)
    {
        // Rotate face
        faces[Facing::Left].rotate();
        // Rotate lines the touch the face as well
        // store original front col
        faces[Facing::Front].copyCol(0, aux);
        // copy up->front
        faces[Facing::Up].copyCol(0, aux2);
        faces[Facing::Front].replaceCol(0, aux2);
        // copy back->up
        faces[Facing::Back].copyCol(2, aux2, true);
        faces[Facing::Up].replaceCol(0, aux2);
        // copy down->back
        faces[Facing::Down].copyCol(0, aux2, true);
        faces[Facing::Back].replaceCol(2, aux2);
        // copy stored front->down
        faces[Facing::Down].replaceCol(0, aux);
    } else if (mod == 1)
    {
        // Rotate face
        faces[Facing::Left].rotate(mod);
        // Rotate lines the touch the face as well
        // store original front col
        faces[Facing::Front].copyCol(0, aux);
        // copy down->front
        faces[Facing::Down].copyCol(0, aux2);
        faces[Facing::Front].replaceCol(0, aux2);
        // copy back->down
        faces[Facing::Back].copyCol(2, aux2, true);
        faces[Facing::Down].replaceCol(0, aux2);
        // copy up->back
        faces[Facing::Up].copyCol(0, aux2, true);
        faces[Facing::Back].replaceCol(2, aux2);
        // copy stored front->up
        faces[Facing::Up].replaceCol(0, aux);
    } else if (mod == 2)
    {
        // Rotate face
        faces[Facing::Left].rotate(mod);
        // Rotate lines the touch the face as well
        // swap front<->back
        faces[Facing::Front].copyCol(0, aux, true);
        faces[Facing::Back].copyCol(2, aux2, true);
        faces[Facing::Front].replaceCol(0, aux2);
        faces[Facing::Back].replaceCol(2, aux);
        // swap up<->down
        faces[Facing::Up].copyCol(0, aux);
        faces[Facing::Down].copyCol(0, aux2);
        faces[Facing::Up].replaceCol(0, aux2);
        faces[Facing::Down].replaceCol(0, aux);
    }
}

void Cube::U(char mod)
{
    unsigned char aux[3];
    unsigned char aux2[3];
    if (!mod)
    {
        // Rotate face
        faces[Facing::Up].rotate();
        // Rotate lines the touch the face as well
        // store original front row
        faces[Facing::Front].copyRow(0, aux);
        // copy right->front
        faces[Facing::Right].copyRow(0, aux2);
        faces[Facing::Front].replaceRow(0, aux2);
        // copy back->right
        faces[Facing::Back].copyRow(0, aux2);
        faces[Facing::Right].replaceRow(0, aux2);
        // copy left->back
        faces[Facing::Left].copyRow(0, aux2);
        faces[Facing::Back].replaceRow(0, aux2);
        // copy stored front->left
        faces[Facing::Left].replaceRow(0, aux);
    } else if (mod == 1)
    {
        // Rotate face
        faces[Facing::Up].rotate(mod);
        // Rotate lines the touch the face as well
        // store original front row
        faces[Facing::Front].copyRow(0, aux);
        // copy left->front
        faces[Facing::Left].copyRow(0, aux2);
        faces[Facing::Front].replaceRow(0, aux2);
        // copy back->left
        faces[Facing::Back].copyRow(0, aux2);
        faces[Facing::Left].replaceRow(0, aux2);
        // copy right->back
        faces[Facing::Right].copyRow(0, aux2);
        faces[Facing::Back].replaceRow(0, aux2);
        // copy stored front->right
        faces[Facing::Right].replaceRow(0, aux);
    } else if (mod == 2)
    {
        // Rotate face
        faces[Facing::Up].rotate(mod);
        // Rotate lines the touch the face as well
        // swap front<->back
        faces[Facing::Front].copyRow(0, aux);
        faces[Facing::Back].copyRow(0, aux2);
        faces[Facing::Front].replaceRow(0, aux2);
        faces[Facing::Back].replaceRow(0, aux);
        // swap right<->left
        faces[Facing::Right].copyRow(0, aux);
        faces[Facing::Left].copyRow(0, aux2);
        faces[Facing::Right].replaceRow(0, aux2);
        faces[Facing::Left].replaceRow(0, aux);
    }
}

void Cube::D(char mod)
{
    unsigned char aux[3];
    unsigned char aux2[3];
    if (!mod)
    {
        // Rotate face
        faces[Facing::Down].rotate();
        // Rotate lines the touch the face as well
        // store original front row
        faces[Facing::Front].copyRow(2, aux);
        // copy left->front
        faces[Facing::Left].copyRow(2, aux2);
        faces[Facing::Front].replaceRow(2, aux2);
        // copy back->left
        faces[Facing::Back].copyRow(2, aux2);
        faces[Facing::Left].replaceRow(2, aux2);
        // copy right->back
        faces[Facing::Right].copyRow(2, aux2);
        faces[Facing::Back].replaceRow(2, aux2);
        // copy stored front->right
        faces[Facing::Right].replaceRow(2, aux);
    } else if (mod == 1)
    {
        // Rotate face
        faces[Facing::Down].rotate(mod);
        // Rotate lines the touch the face as well
        // store original front row
        faces[Facing::Front].copyRow(2, aux);
        // copy right->front
        faces[Facing::Right].copyRow(2, aux2);
        faces[Facing::Front].replaceRow(2, aux2);
        // copy back->right
        faces[Facing::Back].copyRow(2, aux2);
        faces[Facing::Right].replaceRow(2, aux2);
        // copy let->back
        faces[Facing::Left].copyRow(2, aux2);
        faces[Facing::Back].replaceRow(2, aux2);
        // copy stored front->left
        faces[Facing::Left].replaceRow(2, aux);
    } else if (mod == 2)
    {
        // Rotate face
        faces[Facing::Down].rotate(mod);
        // Rotate lines the touch the face as well
        // swap front<->back
        faces[Facing::Front].copyRow(2, aux);
        faces[Facing::Back].copyRow(2, aux2);
        faces[Facing::Front].replaceRow(2, aux2);
        faces[Facing::Back].replaceRow(2, aux);
        // swap right<->left
        faces[Facing::Right].copyRow(2, aux);
        faces[Facing::Left].copyRow(2, aux2);
        faces[Facing::Right].replaceRow(2, aux2);
        faces[Facing::Left].replaceRow(2, aux);
    }
}

void Cube::F(char mod)
{
    unsigned char aux[3];
    unsigned char aux2[3];
    if (!mod)
    {
        // Rotate face
        faces[Facing::Front].rotate();
        // Rotate lines the touch the face as well
        // store original up row
        faces[Facing::Up].copyRow(2, aux);
        // copy left->up
        faces[Facing::Left].copyCol(2, aux2, true);
        faces[Facing::Up].replaceRow(2, aux2);
        // copy down->left
        faces[Facing::Down].copyRow(0, aux2);
        faces[Facing::Left].replaceCol(2, aux2);
        // copy right->down
        faces[Facing::Right].copyCol(0, aux2, true);
        faces[Facing::Down].replaceRow(0, aux2);
        // copy stored up->right
        faces[Facing::Right].replaceCol(0, aux);
    } else if (mod == 1)
    {
        // Rotate face
        faces[Facing::Front].rotate(mod);
        // Rotate lines the touch the face as well
        // store original up row
        faces[Facing::Up].copyRow(2, aux, true);
        // copy right->up
        faces[Facing::Right].copyCol(0, aux2);
        faces[Facing::Up].replaceRow(2, aux2);
        // copy down->right
        faces[Facing::Down].copyRow(0, aux2, true);
        faces[Facing::Right].replaceCol(0, aux2);
        // copy left->down
        faces[Facing::Left].copyCol(2, aux2);
        faces[Facing::Down].replaceRow(0, aux2);
        // copy stored up->left
        faces[Facing::Left].replaceCol(2, aux);
    } else if (mod == 2)
    {
        // Rotate face
        faces[Facing::Front].rotate(mod);
        // Rotate lines the touch the face as well
        // swap up<->down
        faces[Facing::Up].copyRow(2, aux, true);
        faces[Facing::Down].copyRow(0, aux2, true);
        faces[Facing::Up].replaceRow(2, aux2);
        faces[Facing::Down].replaceRow(0, aux);
        // swap right<->left
        faces[Facing::Right].copyCol(0, aux, true);
        faces[Facing::Left].copyCol(2, aux2, true);
        faces[Facing::Right].replaceCol(0, aux2);
        faces[Facing::Left].replaceCol(2, aux);
    }
}

void Cube::B(char mod)
{
    unsigned char aux[3];
    unsigned char aux2[3];
    if (!mod)
    {
        // Rotate face
        faces[Facing::Back].rotate();
        // Rotate lines the touch the face as well
        // store original up row
        faces[Facing::Up].copyRow(0, aux, true);
        // copy right->up
        faces[Facing::Right].copyCol(2, aux2);
        faces[Facing::Up].replaceRow(0, aux2);
        // copy down->right
        faces[Facing::Down].copyRow(2, aux2, true);
        faces[Facing::Right].replaceCol(2, aux2);
        // copy left->down
        faces[Facing::Left].copyCol(0, aux2);
        faces[Facing::Down].replaceRow(2, aux2);
        // copy stored up->left
        faces[Facing::Left].replaceCol(0, aux);
    } else if (mod == 1)
    {
        // Rotate face
        faces[Facing::Back].rotate(mod);
        // Rotate lines the touch the face as well
        // store original up row
        faces[Facing::Up].copyRow(0, aux);
        // copy left->up
        faces[Facing::Left].copyCol(0, aux2, true);
        faces[Facing::Up].replaceRow(0, aux2);
        // copy down->left
        faces[Facing::Down].copyRow(2, aux2);
        faces[Facing::Left].replaceCol(0, aux2);
        // copy right->down
        faces[Facing::Right].copyCol(2, aux2, true);
        faces[Facing::Down].replaceRow(2, aux2);
        // copy stored up->right
        faces[Facing::Right].replaceCol(2, aux);
    } else if (mod == 2)
    {
        // Rotate face
        faces[Facing::Back].rotate(mod);
        // Rotate lines the touch the face as well
        // swap up<->down
        faces[Facing::Up].copyRow(0, aux, true);
        faces[Facing::Down].copyRow(2, aux2, true);
        faces[Facing::Up].replaceRow(0, aux2);
        faces[Facing::Down].replaceRow(2, aux);
        // swap right<->left
        faces[Facing::Right].copyCol(2, aux, true);
        faces[Facing::Left].copyCol(0, aux2, true);
        faces[Facing::Right].replaceCol(2, aux2);
        faces[Facing::Left].replaceCol(0, aux);
    }
}

void Cube::M(char mod)
{
    unsigned char aux[3];
    unsigned char aux2[3];
    // No face to rotate, turn directions as viewed from Left
    if (!mod)
    {
        // store original up col
        faces[Facing::Up].copyCol(1, aux);
        // copy back->up
        faces[Facing::Back].copyCol(1, aux2, true);
        faces[Facing::Up].replaceCol(1, aux2);
        // copy down->back
        faces[Facing::Down].copyCol(1, aux2, true);
        faces[Facing::Back].replaceCol(1, aux2);
        // copy front->down
        faces[Facing::Front].copyCol(1, aux2);
        faces[Facing::Down].replaceCol(1, aux2);
        // copy stored up->front
        faces[Facing::Front].replaceCol(1, aux);
    } else if (mod == 1)
    {
        // store original up col
        faces[Facing::Up].copyCol(1, aux, true);
        // copy front->up
        faces[Facing::Front].copyCol(1, aux2);
        faces[Facing::Up].replaceCol(1, aux2);
        // copy down->front
        faces[Facing::Down].copyCol(1, aux2);
        faces[Facing::Front].replaceCol(1, aux2);
        // copy back->down
        faces[Facing::Back].copyCol(1, aux2, true);
        faces[Facing::Down].replaceCol(1, aux2);
        // copy stored up->back
        faces[Facing::Back].replaceCol(1, aux);
    } else if (mod == 2)
    {
        // swap up<->down
        faces[Facing::Up].copyCol(1, aux);
        faces[Facing::Down].copyCol(1, aux2);
        faces[Facing::Up].replaceCol(1, aux2);
        faces[Facing::Down].replaceCol(1, aux);
        // swap front<->back
        faces[Facing::Front].copyCol(1, aux, true);
        faces[Facing::Back].copyCol(1, aux2, true);
        faces[Facing::Front].replaceCol(1, aux2);
        faces[Facing::Back].replaceCol(1, aux);
    }
}

void Cube::E(char mod)
{
    unsigned char aux[3];
    unsigned char aux2[3];
    // No face to rotate, turn directions as viewed from Down
    if (!mod)
    {
        // store original front row
        faces[Facing::Front].copyRow(1, aux);
        // copy left->front
        faces[Facing::Left].copyRow(1, aux2);
        faces[Facing::Front].replaceRow(1, aux2);
        // copy back->left
        faces[Facing::Back].copyRow(1, aux2);
        faces[Facing::Left].replaceRow(1, aux2);
        // copy right->back
        faces[Facing::Right].copyRow(1, aux2);
        faces[Facing::Back].replaceRow(1, aux2);
        // copy stored front->right
        faces[Facing::Right].replaceRow(1, aux);
    } else if (mod == 1)
    {
        // store original front row
        faces[Facing::Front].copyRow(1, aux);
        // copy right->front
        faces[Facing::Right].copyRow(1, aux2);
        faces[Facing::Front].replaceRow(1, aux2);
        // copy back->right
        faces[Facing::Back].copyRow(1, aux2);
        faces[Facing::Right].replaceRow(1, aux2);
        // copy left->back
        faces[Facing::Left].copyRow(1, aux2);
        faces[Facing::Back].replaceRow(1, aux2);
        // copy stored front->left
        faces[Facing::Left].replaceRow(1, aux);
    } else if (mod == 2)
    {
        // swap right<->left
        faces[Facing::Right].copyRow(1, aux);
        faces[Facing::Left].copyRow(1, aux2);
        faces[Facing::Right].replaceRow(1, aux2);
        faces[Facing::Left].replaceRow(1, aux);
        // swap front<->back
        faces[Facing::Front].copyRow(1, aux);
        faces[Facing::Back].copyRow(1, aux2);
        faces[Facing::Front].replaceRow(1, aux2);
        faces[Facing::Back].replaceRow(1, aux);
    }
}

void Cube::S(char mod)
{
    unsigned char aux[3];
    unsigned char aux2[3];
    // No face to rotate, turn directions as viewed from Front
    if (!mod)
    {
        // store original up row
        faces[Facing::Up].copyRow(1, aux);
        // copy left->up
        faces[Facing::Left].copyCol(1, aux2, true);
        faces[Facing::Up].replaceRow(1, aux2);
        // copy down->left
        faces[Facing::Down].copyRow(1, aux2);
        faces[Facing::Left].replaceCol(1, aux2);
        // copy right->down
        faces[Facing::Right].copyCol(1, aux2, true);
        faces[Facing::Down].replaceRow(1, aux2);
        // copy stored up->right
        faces[Facing::Right].replaceCol(1, aux);
    } else if (mod == 1)
    {
        // store original up row
        faces[Facing::Up].copyRow(1, aux, true);
        // copy right->up
        faces[Facing::Right].copyCol(1, aux2);
        faces[Facing::Up].replaceRow(1, aux2);
        // copy down->right
        faces[Facing::Down].copyRow(1, aux2, true);
        faces[Facing::Right].replaceCol(1, aux2);
        // copy left->down
        faces[Facing::Left].copyCol(1, aux2);
        faces[Facing::Down].replaceRow(1, aux2);
        // copy stored up->left
        faces[Facing::Left].replaceCol(1, aux);
    } else if (mod == 2)
    {
        // swap right<->left
        faces[Facing::Right].copyCol(1, aux, true);
        faces[Facing::Left].copyCol(1, aux2, true);
        faces[Facing::Right].replaceCol(1, aux2);
        faces[Facing::Left].replaceCol(1, aux);
        // swap up<->down
        faces[Facing::Up].copyRow(1, aux, true);
        faces[Facing::Down].copyRow(1, aux2, true);
        faces[Facing::Up].replaceRow(1, aux2);
        faces[Facing::Down].replaceRow(1, aux);
    }
}

void Cube::X(char mod)
{
    unsigned char aux[3][3];
    unsigned char aux2[3][3];
    // Rotate the whole cube on Right
    if (!mod)
    {
        faces[Facing::Right].rotate(mod);
        faces[Facing::Left].rotate(!mod);
        // store original up face
        faces[Facing::Up].copy(aux, true);
        // copy front->up
        faces[Facing::Front].copy(aux2);
        faces[Facing::Up].replace(aux2);
        // copy down->front
        faces[Facing::Down].copy(aux2);
        faces[Facing::Front].replace(aux2);
        // copy back->down
        faces[Facing::Back].copy(aux2, true);
        faces[Facing::Down].replace(aux2);
        // copy stored up->back
        faces[Facing::Back].replace(aux);

    } else if (mod == 1)
    {
        faces[Facing::Right].rotate(mod);
        faces[Facing::Left].rotate(!mod);
        // store original up face
        faces[Facing::Up].copy(aux);
        // copy back->up
        faces[Facing::Back].copy(aux2, true);
        faces[Facing::Up].replace(aux2);
        // copy down->back
        faces[Facing::Down].copy(aux2, true);
        faces[Facing::Back].replace(aux2);
        // copy front->down
        faces[Facing::Front].copy(aux2);
        faces[Facing::Down].replace(aux2);
        // copy stored up->front
        faces[Facing::Front].replace(aux);
    } else if (mod == 2)
    {
        faces[Facing::Right].rotate(mod);
        faces[Facing::Left].rotate(mod);
        // swap front<->back
        faces[Facing::Front].copy(aux, true);
        faces[Facing::Back].copy(aux2, true);
        faces[Facing::Front].replace(aux2);
        faces[Facing::Back].replace(aux);
        // swap up<->down
        faces[Facing::Up].copy(aux);
        faces[Facing::Down].copy(aux2);
        faces[Facing::Up].replace(aux2);
        faces[Facing::Down].replace(aux);
    }
}

void Cube::Y(char mod)
{
    unsigned char aux[3][3];
    unsigned char aux2[3][3];
    // Rotate the whole cube on Up
    if (!mod)
    {
        faces[Facing::Up].rotate(mod);
        faces[Facing::Down].rotate(!mod);
        // store original front face
        faces[Facing::Front].copy(aux);
        // copy right->front
        faces[Facing::Right].copy(aux2);
        faces[Facing::Front].replace(aux2);
        // copy back->right
        faces[Facing::Back].copy(aux2);
        faces[Facing::Right].replace(aux2);
        // copy left->back
        faces[Facing::Left].copy(aux2);
        faces[Facing::Back].replace(aux2);
        // copy stored front->left
        faces[Facing::Left].replace(aux);

    } else if (mod == 1)
    {
        faces[Facing::Up].rotate(mod);
        faces[Facing::Down].rotate(!mod);
        // store original front face
        faces[Facing::Front].copy(aux);
        // copy left->front
        faces[Facing::Left].copy(aux2);
        faces[Facing::Front].replace(aux2);
        // copy back->left
        faces[Facing::Back].copy(aux2);
        faces[Facing::Left].replace(aux2);
        // copy right->back
        faces[Facing::Right].copy(aux2);
        faces[Facing::Back].replace(aux2);
        // copy stored front->right
        faces[Facing::Right].replace(aux);
    } else if (mod == 2)
    {
        faces[Facing::Up].rotate(mod);
        faces[Facing::Down].rotate(mod);
        // swap front<->back
        faces[Facing::Front].copy(aux);
        faces[Facing::Back].copy(aux2);
        faces[Facing::Front].replace(aux2);
        faces[Facing::Back].replace(aux);
        // swap right<->left
        faces[Facing::Right].copy(aux);
        faces[Facing::Left].copy(aux2);
        faces[Facing::Right].replace(aux2);
        faces[Facing::Left].replace(aux);
    }
}

void Cube::Z(char mod)
{
    unsigned char aux[3][3];
    unsigned char aux2[3][3];
    // Rotate the whole cube on Front
    if (!mod)
    {
        faces[Facing::Front].rotate(mod);
        faces[Facing::Back].rotate(!mod);
        // store original up face
        faces[Facing::Up].copy(aux);
        // copy left->up
        faces[Facing::Left].copy(aux2);
        faces[Facing::Up].replace(aux2);
        faces[Facing::Up].rotate();
        // copy down->left
        faces[Facing::Down].copy(aux2);
        faces[Facing::Left].replace(aux2);
        faces[Facing::Left].rotate();
        // copy right->down
        faces[Facing::Right].copy(aux2);
        faces[Facing::Down].replace(aux2);
        faces[Facing::Down].rotate();
        // copy stored up->right
        faces[Facing::Right].replace(aux);
        faces[Facing::Right].rotate();
    } else if (mod == 1)
    {
        faces[Facing::Front].rotate(mod);
        faces[Facing::Back].rotate(!mod);
        // store original up face
        faces[Facing::Up].copy(aux);
        // copy right->up
        faces[Facing::Right].copy(aux2);
        faces[Facing::Up].replace(aux2);
        faces[Facing::Up].rotate(mod);
        // copy down->right
        faces[Facing::Down].copy(aux2);
        faces[Facing::Right].replace(aux2);
        faces[Facing::Right].rotate(mod);
        // copy left->down
        faces[Facing::Left].copy(aux2);
        faces[Facing::Down].replace(aux2);
        faces[Facing::Down].rotate(mod);
        // copy stored up->left
        faces[Facing::Left].replace(aux);
        faces[Facing::Left].rotate(mod);
    } else if (mod == 2)
    {
        faces[Facing::Front].rotate(mod);
        faces[Facing::Back].rotate(mod);
        // swap right<->left
        faces[Facing::Right].copy(aux, true);
        faces[Facing::Left].copy(aux2, true);
        faces[Facing::Right].replace(aux2);
        faces[Facing::Left].replace(aux);
        // swap up<->down
        faces[Facing::Up].copy(aux, true);
        faces[Facing::Down].copy(aux2, true);
        faces[Facing::Up].replace(aux2);
        faces[Facing::Down].replace(aux);
    }
}
