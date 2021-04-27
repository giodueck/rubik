#include "Face.h"
#include <math.h>

Face::Face()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            grid[i][j] = 'a';
}

Face::Face(unsigned char color)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            grid[i][j] = color;
}

Face::Face(unsigned char** grid)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            this->grid[i][j] = grid[i][j];
}

bool Face::isSolved()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            if (grid[i][j] != grid[1][1]) return false;
        }
    
    return true;
}

void Face::copyRow(int r, unsigned char* dest, bool invert)
{
    dest[abs(invert*2 - 0)] = grid[r][0];
    dest[abs(invert*2 - 1)] = grid[r][1];
    dest[abs(invert*2 - 2)] = grid[r][2];
}

void Face::copyCol(int c, unsigned char* dest, bool invert)
{
    dest[abs(invert*2 - 0)] = grid[0][c];
    dest[abs(invert*2 - 1)] = grid[1][c];
    dest[abs(invert*2 - 2)] = grid[2][c];
}

void Face::replaceRow(int r, unsigned char* dest)
{
    grid[r][0] = dest[0];
    grid[r][1] = dest[1];
    grid[r][2] = dest[2];
}

void Face::replaceCol(int c, unsigned char* dest)
{
    grid[0][c] = dest[0];
    grid[1][c] = dest[1];
    grid[2][c] = dest[2];
}

void Face::copy(unsigned char dest[][3], bool invert)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            dest[abs(invert*2 - i)][abs(invert*2 - j)] = grid[i][j];
}

void Face::replace(unsigned char source[][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            grid[i][j] = source[i][j];
}

void Face::rotate(char mod)
{
    unsigned char aux[3][3];
    // copy rotated grid into aux
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (!mod)
            {
                aux[j][2 - i] = this->grid[i][j];
            } else if (mod == 1)
            {
                aux[2 - j][i] = this->grid[i][j];
            } else if (mod == 2)
            {
                aux[2 - i][2 - j] = this->grid[i][j];
            }
        }
    }
    // replace grid with rotated copy
    this->replace(aux);
}
