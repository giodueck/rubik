#ifndef CUBE_H
#define CUBE_H

#include "Face.h"

class Cube
{
    // In the order that enum Facing sets
    Face faces[6];

    public:
        // Start off solved with default colors
        Cube();

        // Start off solved with custom colors, colors has to be at least 6 chars long
        Cube(char* colors);

        // Start off with custom arrangement and colors
        Cube(Face* faces);

        // Functions

        // True if completely solved
        bool isSolved();

        // Scrambles the cube by executing a random assortment of moves
        void scramble(int numMoves = 40, bool print = false);

        // Draws the cube sides laid out flat
        void draw();

        // Copy the cube faces to dest
        void copy(unsigned char dest[6][3][3]);

        // Moves

        // Right
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void R(char mod = 0);
        // Left
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void L(char mod = 0);
        // Up
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void U(char mod = 0);
        // Down
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void D(char mod = 0);
        // Front
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void F(char mod = 0);
        // Back
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void B(char mod = 0);
        
        // Middle
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void M(char mod = 0);
        // Equator
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void E(char mod = 0);
        // Standing
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void S(char mod = 0);
        
        // Rotate on Right
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void X(char mod = 0);
        // Rotate on Up
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void Y(char mod = 0);
        // Rotate on Front
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void Z(char mod = 0);
};

#endif // CUBE_H