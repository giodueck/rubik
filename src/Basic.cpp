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

int Basic::solve(Cube &cube, bool printStats, bool printSteps, int *ctrs, bool dbg)
{
    // Note: the naming of the steps stems from the tendency to orient the cube
    //  white down in humans; the program takes whatever color the center of
    //  the down face has as the 'white' color, and whatever is at the center
    //  of the up face as 'yellow'.

    // Variables
    unsigned char faces[6][3][3];
    unsigned char /* green, blue,*/ yellow /*, white, red, orange*/;
    unsigned char aux, aux2, aux3;
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
                                execute(cube, "B2", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "U R2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U2 F2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "U' L2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Front][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "F D2 F' D2", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "F D' F' D", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                // the piece is in the right position, lookup continues
                                c++;
                                continue;
                            } else // left
                            {
                                execute(cube, "F D F' D'", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Up][1][2];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "R B' R'", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "B U' B' R", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "R' F R", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "U F' L F", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCRtotal, dbg);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCRtotal, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCRtotal, dbg);
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
                                execute(cube, "U B2", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "U2 R2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U' F2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "L2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Left][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "L D L' D'", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "L D2 L' D2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "L D' L' D", WCRtotal, dbg);
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
                                execute(cube, "D2 F D2", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "D' F D", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "F", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "D F D'", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCRtotal, dbg);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCRtotal, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCRtotal, dbg);
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
                                execute(cube, "U' B2", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "R2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U F2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "U2 L2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Down:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Right][2][1];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "R D' R' D", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                // the piece is in the right position, lookup continues
                                c++;
                                continue;
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "R D R' D'", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "R D2 R' D2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Back][1][0];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "B'", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "D B' D'", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "D2 B' D2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "D' B' D", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCRtotal, dbg);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCRtotal, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCRtotal, dbg);
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
                                execute(cube, "U2 B2", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "U' R2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "F2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else
                            {
                                execute(cube, "U L2", WCRtotal, dbg);
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
                                execute(cube, "B D B' D'", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "B D2 B' D2", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "B D' B' D", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Right:
                            // aux is the other face of the edge piece
                            aux = faces[Facing::Down][1][2];
                            if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "R' B'", WCRtotal, dbg);
                                // restart the lookup because the cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Right][1][1])
                            {
                                execute(cube, "R' D' B' D", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "R F", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            } else // left
                            {
                                execute(cube, "R D' F D", WCRtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // rotate cube as the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCRtotal, dbg);
                            // restart the lookup because the cube changed
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCRtotal, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCRtotal, dbg);
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
                                execute(cube, "Y2", WCOtotal, dbg);
                            } else if (!i and j == 2)
                            {
                                execute(cube, "Y", WCOtotal, dbg);
                            } else if (i == 2 and !j)
                            {
                                execute(cube, "Y'", WCOtotal, dbg);
                            }
                            cube.copy(faces);

                            // take note of one of the colors of the edge
                            aux = faces[Facing::Front][0][2];
                            // move corner to the right position
                            if (aux == faces[Facing::Front][1][1])
                            {
                                execute(cube, "U Y'", WCOtotal, dbg);
                            } else if (aux == faces[Facing::Left][1][1])
                            {
                                execute(cube, "U2 Y2", WCOtotal, dbg);
                            } else if (aux == faces[Facing::Back][1][1])
                            {
                                execute(cube, "U' Y", WCOtotal, dbg);
                            }
                            // execute moves to solve the corner
                            execute(cube, moveSets[WCO_U], WCOtotal, dbg);

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
                                    execute(cube, "L' U' L", WCOtotal, dbg);
                                    // restart lookup because cube changed
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            } else if (!i and j == 2)
                            {
                                if (faces[Facing::Front][2][2] == faces[Facing::Front][1][1]) c++;
                                else
                                {
                                    execute(cube, "R U' R'", WCOtotal, dbg);
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            } else if (i == 2 and !j)
                            {
                                if (faces[Facing::Back][2][2] == faces[Facing::Back][1][1]) c++;
                                else
                                {
                                    execute(cube, "L U L'", WCOtotal, dbg);
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
                                    execute(cube, "R' U R", WCOtotal, dbg);
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
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal, dbg);
                                    // restart lookup because cube changed
                                    cube.copy(faces), reset(f, i, j, c);
                                } else if (aux == faces[Facing::Left][1][1])
                                {
                                    execute(cube, "U Y'", WCOtotal, dbg);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal, dbg);
                                    cube.copy(faces), reset(f, i, j, c);
                                } else if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "U2 Y2", WCOtotal, dbg);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal, dbg);
                                    cube.copy(faces), reset(f, i, j, c);
                                } else // right
                                {
                                    execute(cube, "U' Y", WCOtotal, dbg);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal, dbg);
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            } else if (!i and j == 2)
                            {
                                // move to the right face
                                aux = faces[Facing::Back][0][0];
                                if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "U2 Y'", WCOtotal, dbg);
                                    execute(cube, moveSets[MoveSetNames::WCO_F], WCOtotal, dbg);
                                    // restart lookup because cube changed
                                    cube.copy(faces), reset(f, i, j, c);
                                } else if (aux == faces[Facing::Left][1][1])
                                {
                                    execute(cube, "U' Y2", WCOtotal, dbg);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal, dbg);
                                    cube.copy(faces), reset(f, i, j, c);
                                } else if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "Y", WCOtotal, dbg);
                                    execute(cube, moveSets[MoveSetNames::WCO_R], WCOtotal, dbg);
                                    cube.copy(faces), reset(f, i, j, c);
                                } else // right
                                {
                                    execute(cube, "U", WCOtotal, dbg);
                                    execute(cube, moveSets[MoveSetNames::WCO_F], WCOtotal, dbg);
                                    cube.copy(faces), reset(f, i, j, c);
                                }
                            } else if (i == 2 and !j)
                            {
                                // the next two cases require one set of moves to take the corner out
                                // of the spot it is in before solving
                                execute(cube, "R U R' Y'", WCOtotal, dbg);
                                // restart lookup because cube changed
                                cube.copy(faces), reset(f, i, j, c);
                            } else if (i == 2 and j == 2)
                            {
                                execute(cube, "R' U' R Y", WCOtotal, dbg);
                                cube.copy(faces), reset(f, i, j, c);
                            }
                            break;
                        case Facing::Left:
                            // the next cases are permutations of case Facing::Right
                            execute(cube, "Y2", WCOtotal, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Front:
                            execute(cube, "Y'", WCOtotal, dbg);
                            cube.copy(faces), reset(f, i, j, c);
                            break;
                        case Facing::Back:
                            execute(cube, "Y", WCOtotal, dbg);
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
                            execute(cube, "Y2", SLtotal, SLdbg or dbg);
                        } else if (aux2 == faces[Facing::Right][1][1])
                        {
                            execute(cube, "U Y", SLtotal, SLdbg or dbg);
                        } else if (aux2 == faces[Facing::Front][1][1])
                        {
                            execute(cube, "U2", SLtotal, SLdbg or dbg);
                        } else  // left
                        {
                            execute(cube, "U' Y'", SLtotal, SLdbg or dbg);
                        }
                    } else if (!j)      // left
                    {
                        if ((aux2 = faces[Facing::Left][0][1]) == yellow) continue;
                        if (aux2 == faces[Facing::Back][1][1])
                        {
                            execute(cube, "Y2 U", SLtotal, SLdbg or dbg);
                        } else if (aux2 == faces[Facing::Right][1][1])
                        {
                            execute(cube, "U2 Y", SLtotal, SLdbg or dbg);
                        } else if (aux2 == faces[Facing::Front][1][1])
                        {
                            execute(cube, "U'", SLtotal, SLdbg or dbg);
                        } else  // left
                        {
                            execute(cube, "Y'", SLtotal, SLdbg or dbg);
                        }
                    } else if (j == 2)  //right
                    {
                        if ((aux2 = faces[Facing::Right][0][1]) == yellow) continue;
                        if (aux2 == faces[Facing::Back][1][1])
                        {
                            execute(cube, "Y2 U'", SLtotal, SLdbg or dbg);
                        } else if (aux2 == faces[Facing::Right][1][1])
                        {
                            execute(cube, "Y", SLtotal, SLdbg or dbg);
                        } else if (aux2 == faces[Facing::Front][1][1])
                        {
                            execute(cube, "U", SLtotal, SLdbg or dbg);
                        } else  // left
                        {
                            execute(cube, "Y' U2", SLtotal, SLdbg or dbg);
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
                            execute(cube, "Y2 U2", SLtotal, SLdbg or dbg);
                        } else if (aux2 == faces[Facing::Right][1][1])
                        {
                            execute(cube, "Y U'", SLtotal, SLdbg or dbg);
                        } else  // left
                        {
                            execute(cube, "Y' U", SLtotal, SLdbg or dbg);
                        }
                    }
                    cube.copy(faces);

                    // solve edge piece
                    if (aux == faces[Facing::Right][1][1])
                        execute(cube, moveSets[MoveSetNames::SL_R], SLtotal, SLdbg or dbg);
                    else execute(cube, moveSets[MoveSetNames::SL_L], SLtotal, SLdbg or dbg);
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
                            execute(cube, "Y L' U' L U F U F'", SLtotal, SLdbg or dbg);
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
                            execute(cube, "Y2 L' U' L U F U F'", SLtotal, SLdbg or dbg);
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
                            execute(cube, "Y' L' U' L U F U F'", SLtotal, SLdbg or dbg);
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
                            execute(cube, "L' U' L U F U F'", SLtotal, SLdbg or dbg);
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
                            execute(cube, "L' U' L U F U F'", SLtotal, SLdbg or dbg);
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
                            execute(cube, "Y L' U' L U F U F'", SLtotal, SLdbg or dbg);
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
                            execute(cube, "Y2 L' U' L U F U F'", SLtotal, SLdbg or dbg);
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
                            execute(cube, "Y L' U' L U F U F'", SLtotal, SLdbg or dbg);
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
            execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
            execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
            execute(cube, "Y", YCRtotal, dbg);
            execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
            break;
        case 2:
            if (c[0] and c[3]) // line vertical
            {
                execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
            } else if (c[1] and c[2])   // line horizontal
            {
                execute(cube, "Y", YCRtotal, dbg);
                execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
            } else  // 'r'
            {
                if (c[0] and c[1])
                {
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
                } else if (c[0] and c[2])
                {
                    execute(cube,"Y'", YCRtotal, dbg);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
                } else if (c[2] and c[3])
                {
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
                    execute(cube,"Y'", YCRtotal, dbg);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
                } else if (c[1] and c[3])
                {
                    execute(cube,"Y", YCRtotal, dbg);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
                    execute(cube, moveSets[MoveSetNames::YCR], YCRtotal, dbg);
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

    /* Yellow edges */
    {
        bool YEdbg = false;
        // copy cube and look at the edges.
        // if the placement isnt right execute Y and look again, but keep aux and aux2 the same
        // when found check neighboring faces to see how to solve the step
        // step done
        cube.copy(faces);
        // order: right, front, left, back
        std::vector<unsigned char> centers;
        std::vector<bool> solved;
        centers.push_back(faces[Facing::Right][1][1]);
        centers.push_back(faces[Facing::Front][1][1]);
        centers.push_back(faces[Facing::Left][1][1]);
        centers.push_back(faces[Facing::Back][1][1]);
        int mov;
        // look for pair of solved edges
        for (mov = 0; mov < 4; mov++)
        {
            solved.clear();
            solved.push_back(centers[(0 + mov) % 4] == faces[Facing::Right][0][1]);
            solved.push_back(centers[(1 + mov) % 4] == faces[Facing::Front][0][1]);
            solved.push_back(centers[(2 + mov) % 4] == faces[Facing::Left][0][1]);
            solved.push_back(centers[(3 + mov) % 4] == faces[Facing::Back][0][1]);

            if (solved[0] and solved[1] and solved[2] and solved[3])
            {
                switch (mov)
                {
                // case 0:
                //     nop
                //     break;
                case 1:
                    execute(cube, "U Y'", YEtotal, YEdbg or dbg);
                    break;
                case 2:
                    execute(cube, "U2 Y2", YEtotal, YEdbg or dbg);
                    break;
                case 3:
                    execute(cube, "U' Y", YEtotal, YEdbg or dbg);
                    break;
                }
                goto END_YE;
            }
            if (solved[0] and solved[1])
            {
                switch (mov)
                {
                case 0:
                    execute(cube, "Y'", YEtotal, YEdbg or dbg);
                    break;
                case 1:
                    execute(cube, "U Y2", YEtotal, YEdbg or dbg);
                    break;
                case 2:
                    execute(cube, "U2 Y", YEtotal, YEdbg or dbg);
                    break;
                case 3:
                    execute(cube, "U'", YEtotal, YEdbg or dbg);
                    break;
                }
                goto LSHAPE;
            } else if (solved[1] and solved[2])
            {
                switch (mov)
                {
                case 0:
                    execute(cube, "Y2", YEtotal, YEdbg or dbg);
                    break;
                case 1:
                    execute(cube, "U Y", YEtotal, YEdbg or dbg);
                    break;
                case 2:
                    execute(cube, "U2", YEtotal, YEdbg or dbg);
                    break;
                case 3:
                    execute(cube, "U' Y'", YEtotal, YEdbg or dbg);
                    break;
                }
                goto LSHAPE;
            } else if (solved[2] and solved[3])
            {
                switch (mov)
                {
                case 0:
                    execute(cube, "Y", YEtotal, YEdbg or dbg);
                    break;
                case 1:
                    execute(cube, "U", YEtotal, YEdbg or dbg);
                    break;
                case 2:
                    execute(cube, "U2 Y'", YEtotal, YEdbg or dbg);
                    break;
                case 3:
                    execute(cube, "U' Y2", YEtotal, YEdbg or dbg);
                    break;
                }
                goto LSHAPE;
            } else if (solved[3] and solved[0])
            {
                switch (mov)
                {
                // case 0:
                //     nop
                //     break;
                case 1:
                    execute(cube, "U Y'", YEtotal, YEdbg or dbg);
                    break;
                case 2:
                    execute(cube, "U2 Y2", YEtotal, YEdbg or dbg);
                    break;
                case 3:
                    execute(cube, "U' Y", YEtotal, YEdbg or dbg);
                    break;
                }
                goto LSHAPE;
            } else if (solved[0] and solved[2])
            {
                switch (mov)
                {
                // case 0:
                //     // nop
                //     break;
                case 1:
                    execute(cube, "U Y'", YEtotal, YEdbg or dbg);
                    break;
                }
                goto LINE;
            } else if (solved[1] and solved[3])
            {
                switch (mov)
                {
                case 0:
                    execute(cube, "Y", YEtotal, YEdbg or dbg);
                    break;
                case 1:
                    execute(cube, "U", YEtotal, YEdbg or dbg);
                    break;
                }
                goto LINE;
            }
        }

        LINE:
        execute(cube, moveSets[MoveSetNames::YE_O], YEtotal, YEdbg or dbg);
        goto END_YE;

        LSHAPE:
        execute(cube, moveSets[MoveSetNames::YE_A], YEtotal, YEdbg or dbg);
        goto END_YE;
    }
    END_YE:
    if (printSteps)
    {
        std::cout << "Yellow edges:\n";
        cube.draw();
        std::cout << '\n';
    }

    /* Yellow corner placement */
    {
        // loop through up corners to see if they aare in the right place
        // if none, do moveset to get one
        // if only one, do moveset to see if step done
        // if all, step done
        cube.copy(faces);
        unsigned char front, right, left;
        yellow = faces[Facing::Up][1][1];
        // to get out of the while use goto
        while (true)
        {
            for (int i = 0; i < 4; i++)
            {
                // these are here for readability
                front = faces[Facing::Front][1][1];
                right = faces[Facing::Right][1][1];
                left = faces[Facing::Left][1][1];
                aux = faces[Facing::Up][2][2];
                aux2 = faces[Facing::Front][0][2];
                aux3 = faces[Facing::Right][0][0];

                // check if the corner has the right colors
                if ((aux == yellow or aux == front or aux == right) and 
                    (aux2 == yellow or aux2 == front or aux2 == right) and 
                    (aux3 == yellow or aux3 == front or aux3 == right))
                {
                    // check if done
                    aux = faces[Facing::Up][2][0];
                    aux2 = faces[Facing::Front][0][0];
                    aux3 = faces[Facing::Left][0][2];
                    if ((aux == yellow or aux == front or aux == left) and 
                        (aux2 == yellow or aux2 == front or aux2 == left) and 
                        (aux3 == yellow or aux3 == front or aux3 == left))
                    {
                        goto END_YCO;
                    } else
                    {
                        execute(cube, moveSets[MoveSetNames::YCO], YCOtotal, dbg);
                        cube.copy(faces);
                    }
                // if not rotate the cube
                } else
                {
                    execute(cube, "Y", YCOtotal, dbg);
                    cube.copy(faces);
                }
            }
            // if no correct corners were found execute the moves once and restart
            execute(cube, moveSets[MoveSetNames::YCO], YCOtotal, dbg);
            cube.copy(faces);
        }

    }
    END_YCO:
    if (printSteps)
    {
        std::cout << "Yellow corners:\n";
        cube.draw();
        std::cout << '\n';
    }

    /* Yellow corner orientation */
    {
        // check which corners arent solved. rotate the cube to the first one
        // do corner orientation moveset
        // check if solved, if not do again, if yes, move up to the next unsolved corner or finish
        cube.copy(faces);
        yellow = faces[Facing::Up][1][1];
        int i = 0, j = 0;
        while (true)
        {
            if (faces[Facing::Up][2][2] != yellow)
            {
                execute(cube, moveSets[MoveSetNames::YCOO], YCOOtotal, dbg);
                i++;
            } else if (faces[Facing::Up][0][2] != yellow)
            {
                if (i % 3 != 0)
                {
                    execute(cube, "U", YCOOtotal, dbg);
                    j += 1;
                } else execute(cube, "Y", YCOOtotal, dbg);
            } else if (faces[Facing::Up][0][0] != yellow)
            {
                if (i % 3 != 0)
                {
                    execute(cube, "U2", YCOOtotal, dbg);
                    j += 2;
                } else execute(cube, "Y2", YCOOtotal, dbg);
            } else if (faces[Facing::Up][2][0] != yellow)
            {
                if (i % 3 != 0)
                {
                    execute(cube, "U'", YCOOtotal, dbg);
                    j += 3;
                } else execute(cube, "Y'", YCOOtotal, dbg);
            } else goto REPAIR_UP;
            cube.copy(faces);
        }

        REPAIR_UP:
        switch (j)
        {
        case 0:
            break;
        case 1:
            execute(cube, "U'", YCOOtotal, dbg);
            break;
        case 2:
            execute(cube, "U2", YCOOtotal, dbg);
            break;
        case 3:
            execute(cube, "U", YCOOtotal, dbg);
            break;
        }
    }
    END_YCOO:
    if (printSteps)
    {
        std::cout << "Yellow corner orientation:\n";
        cube.draw();
        std::cout << '\n';
    }

    /* Count up moves and print stats*/
    {
        std::cout << "\nSolved!\n";
        if (!printSteps)
        {
            cube.draw();
            std::cout << '\n';
        }

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
            std::cout << '\t' << "_________________________________\n";
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