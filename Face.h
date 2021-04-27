#ifndef FACE_H
#define FACE_H

enum Facing { Right, Left, Up, Down, Front, Back };

class Face
{
    unsigned char grid[3][3];

    public:
        // Default (blank) constructor
        Face();

        // Constructor, color is the symbol to be used
        Face(unsigned char color);

        // grid is a matrix of 3x3
        Face(unsigned char** grid);

        // true if all pieces are the same as the center piece
        bool isSolved();

        // Copies out the colors of a row r (0-2) into dest, which is an array of 3 elements.
        // For some moves an inverted copy is needed, set invert accordingly
        void copyRow(int r, unsigned char* dest, bool invert = false);

        // Copies out the colors of a column c (0-2) into dest, which is an array of 3 elements.
        // For some moves an inverted copy is needed, set invert accordingly
        void copyCol(int c, unsigned char* dest, bool invert = false);

        // Copies in the colors of a row r (0-2) from dest, which is an array of 3 elements.
        void replaceRow(int r, unsigned char* dest);

        // Copies in the colors of a column c (0-2) from dest, which is an array of 3 elements.
        void replaceCol(int c, unsigned char* dest);

        // Copies the 3x3 face into dest, has to be a matrix of at least 3x3
        // For some moves an inverted copy is needed, set invert accordingly
        void copy(unsigned char dest[][3], bool invert = false);

        // Copies the given 3x3 matrix into the face
        void replace(unsigned char source[][3]);

        // Rotates this face in the desired direction, does not affect other faces though!
        // mod: 0 is clockwise, 1 is counterclockwise, 2 is double
        void rotate(char mod = 0);
};

#endif // FACE_H