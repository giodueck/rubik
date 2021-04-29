#include "Basic.h"
#include <iostream>

Basic::Basic()
{
    // white corners
    moveSets.push_back("R U R'");       //fru facing right
    moveSets.push_back("F' U' F");      //fru facing front
    moveSets.push_back("R B U2 B' R'"); //fru facing up
    // 2nd layer
    moveSets.push_back("U R U' R' F R' F' R");  // fu to fr
    moveSets.push_back("U' L' U L F' L F L'");  // fu to fl
    // yellow cross
    moveSets.push_back("R' F' U' F U R");
    // yellow edges
    moveSets.push_back("R U R' U R U2 R' U");                   // adjacent solved
    moveSets.push_back("R U R' U R U2 R' U' R U R' U R U2 R'"); // opposing solved
    // yellow corner placement
    moveSets.push_back("U R U' L' U R' U' L");
    // yellow corner orientation
    moveSets.push_back("R' D' R D R' D' R D");
}

void reset(int &f, int &i, int &j, int &c)
{
    f = c = i = 0;
    j = -1;
}

int Basic::solve(Cube &cube, bool printStats, bool printSteps, int *ctrs)
{
    // Note: the naming of the steps stems from the tendency to orient the cube
    //  white down in humans; the program takes whatever color the center of
    //  the down face has as the 'white' color, and whatever is at the center
    //  of the up face as 'yellow'.

    // Variables
    unsigned char faces[6][3][3];
    unsigned char /* green, blue,*/ yellow /*, white, red, orange*/;
    unsigned char aux, aux2;
    int total = 0, WCRtotal = 0, WCOtotal = 0, SLtotal = 0, YCRtotal = 0, YEtotal = 0, YCOtotal = 0, YCOOtotal = 0;

    /* White cross */
    {
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
                                execute(cube, "B2", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "U R2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U2 F2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "U' L2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Front][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "F D2 F' D2", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "F D' F' D", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                // the piece is in the right position, lookup continues
                                c++;
                                continue;
                            } else // left
                            {
                                execute(cube, "F D F' D'", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Up][1][2];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "R B' R'", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "B U' B' R", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "R' F R", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "U F' L F", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCRtotal);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCRtotal);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCRtotal);
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
                                execute(cube, "U B2", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "U2 R2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U' F2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "L2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Left][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "L D L' D'", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "L D2 L' D2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "L D' L' D", WCRtotal);
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
                                execute(cube, "D2 F D2", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "D' F D", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "F", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "D F D'", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCRtotal);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCRtotal);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCRtotal);
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
                                execute(cube, "U' B2", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "R2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U F2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "U2 L2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Right][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "R D' R' D", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                // the piece is in the right position, lookup continues
                                c++;
                                continue;
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "R D R' D'", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "R D2 R' D2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Back][1][0];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "B'", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "D B' D'", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "D2 B' D2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "D' B' D", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCRtotal);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCRtotal);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCRtotal);
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
                                execute(cube, "U2 B2", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "U' R2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "F2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "U L2", WCRtotal);
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
                                if (c == 4) goto END_WCR;
                                continue;
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "B D B' D'", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "B D2 B' D2", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "B D' B' D", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Down][1][2];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "R' B'", WCRtotal);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "R' D' B' D", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "R F", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "R D' F D", WCRtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCRtotal);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCRtotal);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCRtotal);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        }
                    }
                }
    }
    END_WCR:
    if (printSteps)
    {
        std::cout << "White cross:\n";
        cube.draw();
        std::cout << '\n';
    }

    /* White corners */
    {
        // look up corners
        // if on fru execute moves
        // else move to fru if not already set
        cube.copy(faces);
        for (int f = 0, c = 0; f < 6; f++)
            for (int i = 0; i < 3; i += 2)
                for (int j = 0; j < 3; j++)
                {
                    // only check corners
                    if (j % 2) continue;
                    // check if the piece is white
                    if (faces[f][i][j] == faces[Facing::Down][1][1])
                    {
                        switch (f)
                        {
                        case Facing::Up:
                            // rotate the appropriate face to front
                            if (!i and !j)
                            {
                                execute(cube, "Y2", WCOtotal);
                            } else if (!i and j == 2)
                            {
                                execute(cube, "Y", WCOtotal);
                            } else if (i == 2 and !j)
                            {
                                execute(cube, "Y'", WCOtotal);
                            }
                            cube.copy(faces);

                            // take note of one of the colors of the edge
                            aux = faces[Facing::Front][0][2];
                            // move corner to the right position
                            if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U Y'", WCOtotal);
                            } else if (aux == faces[Facing::Left][1][1])
                            {
                                execute(cube, "U2 Y2", WCOtotal);
                            } else if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "U' Y", WCOtotal);
                            }
                            // execute moves to solve the corner
                            execute(cube, moveSets[WCO_U], WCOtotal);

                            // restart lookup because cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Down:
                            // check if corner is in the right spot, if not move to frd and move curner out
                            if (!i and !j)
                            {
                                if (faces[Facing::Front][2][0] == faces[Facing::Front][1][1]) c++;
                                else
                                {
                                    execute(cube, "L' U' L", WCOtotal);
                                    // restart lookup because cube changed
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            } else if (!i and j == 2)
                            {
                                if (faces[Facing::Front][2][2] == faces[Facing::Front][1][1]) c++;
                                else
                                {
                                    execute(cube, "R U' R'", WCOtotal);
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            } else if (i == 2 and !j)
                            {
                                if (faces[Facing::Back][2][2] == faces[Facing::Back][1][1]) c++;
                                else
                                {
                                    execute(cube, "L U L'", WCOtotal);
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            } else if (i == 2 and j == 2)
                            {
                                if (faces[Facing::Back][2][0] == faces[Facing::Back][1][1])
                                {
                                    c++;
                                    // check if step is complete
                                    if (c == 4) goto END_WCO;
                                }
                                else
                                {
                                    execute(cube, "R' U R", WCOtotal);
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            }
                            break;
                        case Facing::Right:
                            // each position has a set of moves to solve the corner
                            if (!i and !j)
                            {
                                // move to the right face
                                aux = faces[Facing::Front][0][2];
                                if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal);
                                    // restart lookup because cube changed
                                    cube.copy(faces), reset(f, i, j, c);
                                } else if (aux == faces[Facing::Left][1][1])
                                {
                                    execute(cube, "U Y'", WCOtotal);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal);
                                    cube.copy(faces), reset(f, i, j, c);
                                } else if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "U2 Y2", WCOtotal);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal);
                                    cube.copy(faces), reset(f, i, j, c);
                                } else // right
                                {
                                    execute(cube, "U' Y", WCOtotal);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal);
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            } else if (!i and j == 2)
                            {
                                // move to the right face
                                aux = faces[Facing::Back][0][0];
                                if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "U2 Y'", WCOtotal);
                                    execute(cube, moveSets[MoveSetNames::WCO_F], WCOtotal);
                                    // restart lookup because cube changed
                                    cube.copy(faces), reset(f, i, j, c);
                                } else if (aux == faces[Facing::Left][1][1])
                                {
                                    execute(cube, "U' Y2", WCOtotal);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal);
                                    cube.copy(faces), reset(f, i, j, c);
                                } else if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "Y", WCOtotal);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal);
                                    cube.copy(faces), reset(f, i, j, c);
                                } else // right
                                {
                                    execute(cube, "U", WCOtotal);
                                    execute(cube, moveSets[MoveSetNames::WCO_F], WCOtotal);
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            } else if (i == 2 and !j)
                            {
                                // the next two cases require one set of moves to take the corner out
                                // of the spot it is in before solving
                                execute(cube, "R U R' Y'", WCOtotal);
                                // restart lookup because cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (i == 2 and j == 2)
                            {
                                execute(cube, "R' U' R Y", WCOtotal);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCOtotal);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCOtotal);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCOtotal);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        }
                    }
                }
    }
    END_WCO:
    if (printSteps)
    {
        std::cout << "White corners:\n";
        cube.draw();
        std::cout << '\n';
    }

    /* Second layer */
    {
        bool SLdbg = false;
        // look at up for non-yellow edges, if found insert
        // if not look at sides for edges that dont match center and move them out
        // if none found 2L is done
        cube.copy(faces);
        yellow = faces[Facing::Up][1][1];
        for (int i = 0, f = Facing::Up, c = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                // only evaluate edges (i + j = uneven)
                if (!((i + j) % 2)) continue;

                cube.copy(faces);
                aux = faces[f][i][j];
                switch (f)
                {
                case Facing::Up:
                    if (aux == faces[Facing::Up][1][1])
                    {
                        if (i == 2) f = Facing::Right, reset(j, i, j, c);
                        continue;
                    }
                    // rotate piece to correct face and front
                    if (!i)             // back
                    {
                        if ((aux2 = faces[Facing::Back][0][1]) == yellow) continue;
                        if (aux2 == faces[Facing::Back][1][1])
                        {
                            execute(cube, "Y2", SLtotal, SLdbg);
                        } else if (aux2 == faces[Facing::Right][1][1])
                        {
                            execute(cube, "U Y", SLtotal, SLdbg);
                        } else if (aux2 == faces[Facing::Front][1][1])
                        {
                            execute(cube, "U2", SLtotal, SLdbg);
                        } else  // left
                        {
                            execute(cube, "U' Y'", SLtotal, SLdbg);
                        }
                    } else if (!j)      // left
                    {
                        if ((aux2 = faces[Facing::Left][0][1]) == yellow) continue;
                        if (aux2 == faces[Facing::Back][1][1])
                        {
                            execute(cube, "Y2 U", SLtotal, SLdbg);
                        } else if (aux2 == faces[Facing::Right][1][1])
                        {
                            execute(cube, "U2 Y", SLtotal, SLdbg);
                        } else if (aux2 == faces[Facing::Front][1][1])
                        {
                            execute(cube, "U'", SLtotal, SLdbg);
                        } else  // left
                        {
                            execute(cube, "Y'", SLtotal, SLdbg);
                        }
                    } else if (j == 2)  //right
                    {
                        if ((aux2 = faces[Facing::Right][0][1]) == yellow) continue;
                        if (aux2 == faces[Facing::Back][1][1])
                        {
                            execute(cube, "Y2 U'", SLtotal, SLdbg);
                        } else if (aux2 == faces[Facing::Right][1][1])
                        {
                            execute(cube, "Y", SLtotal, SLdbg);
                        } else if (aux2 == faces[Facing::Front][1][1])
                        {
                            execute(cube, "U", SLtotal, SLdbg);
                        } else  // left
                        {
                            execute(cube, "Y' U2", SLtotal, SLdbg);
                        }
                    } else if (i == 2)  //front
                    {
                        if ((aux2 = faces[Facing::Front][0][1]) == yellow)
                        {
                            f = Facing::Right, reset(j, i, j, c);
                            continue;
                        }
                        if (aux2 == faces[Facing::Back][1][1])
                        {
                            execute(cube, "Y2 U2", SLtotal, SLdbg);
                        } else if (aux2 == faces[Facing::Right][1][1])
                        {
                            execute(cube, "Y U'", SLtotal, SLdbg);
                        } else  // left
                        {
                            execute(cube, "Y' U", SLtotal, SLdbg);
                        }
                    }
                    cube.copy(faces);

                    // solve edge piece
                    if (aux == faces[Facing::Right][1][1])
                        execute(cube, moveSets[MoveSetNames::SL_R], SLtotal, SLdbg);
                    else execute(cube, moveSets[MoveSetNames::SL_L], SLtotal, SLdbg);
                    // restart lookup because cube changed, but set f to Up
                    f = Facing::Up, reset(j, i, j, c), cube.copy(faces);
                    break;
                case Facing::Right:
                    // for the sides look at edges to the left and right
                    // if correct color increment c, if c == 8 step done
                    // if not move out and set f to Up
                    if (!j) // left
                    {
                        if (aux == faces[f][1][1]) c++;
                        else
                        {
                            execute(cube, "Y L' U' L U F U F'", SLtotal, SLdbg);
                            f = Facing::Up, reset(j, i, j, c), cube.copy(faces);
                        }
                    } else if (j == 2)  // right
                    {
                        if (aux == faces[f][1][1])
                        {
                            c++;
                            f = Facing::Left, i = 0, j = -1;
                        }
                        else
                        {
                            execute(cube, "Y2 L' U' L U F U F'", SLtotal, SLdbg);
                            f = Facing::Up, reset(j, i, j, c), cube.copy(faces);
                        }
                    }
                    break;
                case Facing::Left:
                    if (!j) // left
                    {
                        if (aux == faces[f][1][1]) c++;
                        else
                        {
                            execute(cube, "Y' L' U' L U F U F'", SLtotal, SLdbg);
                            f = Facing::Up, reset(j, i, j, c), cube.copy(faces);
                        }
                    } else if (j == 2)  // right
                    {
                        if (aux == faces[f][1][1])
                        {
                            c++;
                            f = Facing::Front, i = 0, j = -1;
                        }
                        else
                        {
                            execute(cube, "L' U' L U F U F'", SLtotal, SLdbg);
                            f = Facing::Up, reset(j, i, j, c), cube.copy(faces);
                        }
                    }
                    break;
                case Facing::Front:
                    if (!j) // left
                    {
                        if (aux == faces[f][1][1]) c++;
                        else
                        {
                            execute(cube, "L' U' L U F U F'", SLtotal, SLdbg);
                            f = Facing::Up, reset(j, i, j, c), cube.copy(faces);
                        }
                    } else if (j == 2)  // right
                    {
                        if (aux == faces[f][1][1])
                        {
                            c++;
                            f = Facing::Back, i = 0, j = -1;
                        }
                        else
                        {
                            execute(cube, "Y L' U' L U F U F'", SLtotal, SLdbg);
                            f = Facing::Up, reset(j, i, j, c), cube.copy(faces);
                        }
                    }
                    break;
                case Facing::Back:
                    if (!j) // left
                    {
                        if (aux == faces[f][1][1]) c++;
                        else
                        {
                            execute(cube, "Y2 L' U' L U F U F'", SLtotal, SLdbg);
                            f = Facing::Up, reset(j, i, j, c), cube.copy(faces);
                        }
                    } else if (j == 2)  // right
                    {
                        if (aux == faces[f][1][1])
                        {
                            c++;
                            if (c == 8) goto END_SL;
                        }
                        else
                        {
                            execute(cube, "Y L' U' L U F U F'", SLtotal, SLdbg);
                            f = Facing::Up, reset(j, i, j, c), cube.copy(faces);
                        }
                    }
                    break;
                
                }
            }
    }
    END_SL:
    if (printSteps)
    {
        std::cout << "Second layer:\n";
        cube.draw();
        std::cout << '\n';
    }

    /* Yellow cross */
    {
        // check edges for shapes:
        // if dot, 'r' or line, orient cube and do moveset
        // if cross step is done

        std::vector<bool> c;
        cube.copy(faces);
        yellow = faces[Facing::Up][1][1];
        c.push_back(faces[Facing::Up][0][1] == yellow);
        c.push_back(faces[Facing::Up][1][0] == yellow);
        c.push_back(faces[Facing::Up][1][2] == yellow);
        c.push_back(faces[Facing::Up][2][1] == yellow);
        
        switch (c[0] + c[1] + c[2] + c[3])
        {
        case 0:
            execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
            execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
            execute(cube, "Y", YCRtotal);
            execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
            break;
        case 2:
            if (c[0] and c[3]) // line vertical
            {
                execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
            } else if (c[1] and c[2])   // line horizontal
            {
                execute(cube, "Y", YCRtotal);
                execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
            } else  // 'r'
            {
                if (c[0] and c[1])
                {
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
                } else if (c[0] and c[2])
                {
                    execute(cube,"Y'", YCRtotal);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
                } else if (c[2] and c[3])
                {
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
                    execute(cube,"Y'", YCRtotal);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
                } else if (c[1] and c[3])
                {
                    execute(cube,"Y", YCRtotal);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal);
                }
            }
            break;
        }
    }
    END_YCR:
    if (printSteps)
    {
        std::cout << "Yellow cross:\n";
        cube.draw();
        std::cout << '\n';
    }

    /* Count up moves and print stats*/
    {
        std::cout << "\nSolved!\n";
        cube.draw();
        std::cout << '\n';

        if (ctrs)
        {
            ctrs[0] = WCRtotal;
            ctrs[1] = WCOtotal;
            ctrs[2] = SLtotal;
            ctrs[3] = YCRtotal;
            ctrs[4] = YEtotal;
            ctrs[5] = YCOtotal;
            ctrs[6] = YCOOtotal;
        }
        if (printStats)
        {
            std::cout << "\nStats:\n";
            std::cout << '\t' << WCRtotal << " - White cross\n";
            std::cout << '\t' << WCOtotal << " - White corners\n";
            std::cout << '\t' << SLtotal << " - Second layer\n";
            std::cout << '\t' << YCRtotal << " - Yellow cross\n";
            std::cout << '\t' << YEtotal << " - Yellow edges\n";
            std::cout << '\t' << YCOtotal << " - Yellow corners\n";
            std::cout << '\t' << YCOOtotal << " - Yellow corner orientations\n";
            std::cout << '\t' << "_________________________________";
            std::cout << '\t' << (total = WCRtotal + WCOtotal + SLtotal + YCRtotal + YEtotal + YCOtotal + YCOOtotal) << " - Total moves\n\n";
        }
        return total;
    }
}

void Basic::execute(Cube &cube, std::string sequence, int &ctr, bool debug)
{
    if (debug) std::cout << "\tExecute " << sequence << '\n';
    Parser::parse(cube, sequence, ctr, false, debug);
}