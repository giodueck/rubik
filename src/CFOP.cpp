#include "CFOP.h"
#include <iostream>

void reset(int &f, int &i, int &j, int &c)
{
    f = c = i = 0;
    j = -1;
}

CFOP::CFOP() {}

void CFOP::C(Cube &cube, int &ctr, bool dbg)
{
    // Code identical to the code in Beginner
    unsigned char faces[6][3][3];
    unsigned char aux;

        // look for white edges iteratively
        cube.copy(faces);
        for (int f = 0, c = 0; f < 6; f++)
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    // only evaluate edges (i + j = uneven)
                    if (!((i + j) % 2)) continue;
                    // if the faces are not white do nothing
                    if (faces[f][i][j] != faces[Facing::Down][1][1]) continue;

                    if (!i) // on the top of the face
                    {
                        // Moves are different depending on which face the white side of the edge is
                        switch (f)
                        {
                        case Facing::Up:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Back][0][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "B2", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "U R2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U2 F2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "U' L2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Front][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "F D2 F' D2", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "F D' F' D", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                // the piece is in the right position, lookup continues
                                c++;
                                continue;
                            } else // left
                            {
                                execute(cube, "F D F' D'", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Up][1][2];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "R B' R'", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "B U' B' R", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "R' F R", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "U F' L F", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", ctr, dbg);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", ctr, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", ctr, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        }
                    } else if (!j)  // on the left of the face
                    {
                        // Moves are different depending on which face the white side of the edge is
                        switch (f)
                        {
                        case Facing::Up:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Left][0][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "U B2", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "U2 R2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U' F2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "L2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Left][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "L D L' D'", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "L D2 L' D2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "L D' L' D", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                // the piece is in the right position, lookup continues
                                c++;
                                continue;
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Front][1][2];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "D2 F D2", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "D' F D", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "F", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "D F D'", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", ctr, dbg);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", ctr, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", ctr, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        }
                    } else if (j == 2)  // on the right of the face
                    {
                        // Moves are different depending on which face the white side of the edge is
                        switch (f)
                        {
                        case Facing::Up:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Right][0][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "U' B2", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "R2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U F2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "U2 L2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Right][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "R D' R' D", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                // the piece is in the right position, lookup continues
                                c++;
                                continue;
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "R D R' D'", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "R D2 R' D2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Back][1][0];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "B'", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "D B' D'", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "D2 B' D2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "D' B' D", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", ctr, dbg);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", ctr, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", ctr, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        }
                    } else if (i == 2)  // on the bottom of the face
                    {
                        // Moves are different depending on which face the white side of the edge is
                        switch (f)
                        {
                        case Facing::Up:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Front][0][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "U2 B2", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "U' R2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "F2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "U L2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                        // aux is the other face of the edge piece
                            aux = faces[Facing::Back][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                // the piece is in the right position, lookup continues
                                c++;
                                // if the cross is complete exit
                                if (c == 4) return;
                                continue;
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "B D B' D'", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "B D2 B' D2", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "B D' B' D", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Down][1][2];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "R' B'", ctr, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "R' D' B' D", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "R F", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "R D' F D", ctr, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", ctr, dbg);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", ctr, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", ctr, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        }
                    }
                }
}

void CFOP::F(Cube &cube, int &ctr, bool dbg)
{
    
}

void CFOP::O(Cube &cube, int &ctr, bool dbg)
{
    
}

void CFOP::P(Cube &cube, int &ctr, bool dbg)
{
    
}

S_CFOP::S_CFOP() {}

int S_CFOP::solve(Cube &cube, bool printStats, bool printSteps, int *ctrs, bool dbg)
{
    /*
        Steps:
            Cross: regular
            F2L: regular
            Orient: yellow cross, yellow corners
            Permute: setup, solve (A perm)
    */
    int ctrduds[4] = {0};
    if (ctrs == NULL) ctrs = ctrduds;

    C(cube, ctrs[0], dbg);
    if (printSteps)
    {
        std::cout << "Cross:\n";
        cube.draw();
        std::cout << '\n';
    }

    F(cube, ctrs[1], dbg);
    if (printSteps)
    {
        std::cout << "First two layers:\n";
        cube.draw();
        std::cout << '\n';
    }

    O(cube, ctrs[2], dbg);
    if (printSteps)
    {
        std::cout << "Orient last layer:\n";
        cube.draw();
        std::cout << '\n';
    }

    P(cube, ctrs[3], dbg);
    if (printSteps)
    {
        std::cout << "Permute last layers:\n";
        cube.draw();
        std::cout << '\n';
    }
    int total = ctrs[0] + ctrs[1] + ctrs[2] + ctrs[3];
    if (printStats)
    {
        std::cout << "\nStats:\n";
        std::cout << '\t' << ctrs[0] << " - Cross\n";
        std::cout << '\t' << ctrs[1] << " - First two layers\n";
        std::cout << '\t' << ctrs[2] << " - Orient last layers\n";
        std::cout << '\t' << ctrs[3] << " - Permute last layers\n";
        std::cout << '\t' << "_________________________________\n";
        std::cout << '\t' << total << " - Total moves\n\n";
    }
    return total;
}

void S_CFOP::O(Cube &cube, int &ctr, bool dbg)
{

}

void S_CFOP::P(Cube &cube, int &ctr, bool dbg)
{

}