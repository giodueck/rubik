#include "Basic.h"
#include <iostream>

Basic::Basic()
{
    // white corners
    moveSets.push_back("R U R'");       //fru facing front
    moveSets.push_back("F' U' F");      //flu facing front
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

int Basic::solve(Cube &cube, bool printStats, bool printSteps, int *ctrs)
{
    // Note: the naming of the steps stems from the tendency to orient the cube
    //  white down in humans; the program takes whatever color the center of
    //  the down face has as the 'white' color, and whatever is at the center
    //  of the up face as 'yellow'.

    // Variables
    unsigned char faces[6][3][3];
    // unsigned char green, blue, yellow, white, red, orange;
    unsigned char aux;
    int total = 0, WCRtotal = 0, WCOtotal = 0, SLtotal = 0, YCRtotal = 0,
        YEtotal = 0, YCOtotal = 0, YCOOtotal = 0;

    // take note of which face is which "color"
    // cube.copy(faces);
    // green = faces[Facing::Right][1][1];
    // blue = faces[Facing::Left][1][1];
    // yellow = faces[Facing::Up][1][1];
    // white = faces[Facing::Down][1][1];
    // red = faces[Facing::Front][1][1];
    // orange = faces[Facing::Back][1][1];

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
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "U R2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "U2 F2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else
                                {
                                    execute(cube, "U' L2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Down:
                                // aux is the other face of the edge piece
                                aux = faces[Facing::Front][2][1];
                                if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "F D2 F' D2", WCRtotal);
                                    // restart the lookup because the cube changed
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "F D' F' D", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    // the piece is in the right position, lookup continues
                                    c++;
                                    continue;
                                } else // left
                                {
                                    execute(cube, "F D F' D'", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Right:
                                // aux is the other face of the edge piece
                                aux = faces[Facing::Up][1][2];
                                if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "R B' R'", WCRtotal);
                                    // restart the lookup because the cube changed
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "B U' B' R", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "R' F R", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else // left
                                {
                                    execute(cube, "U F' L F", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Left:
                                // rotate cube as the next cases are permutations of case Facing::Right
                                execute(cube, "Y2", WCRtotal);
                                // restart the lookup because the cube changed
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                break;
                            case Facing::Front:
                                execute(cube, "Y'", WCRtotal);
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                break;
                            case Facing::Back:
                                execute(cube, "Y", WCRtotal);
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
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
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "U2 R2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "U' F2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else
                                {
                                    execute(cube, "L2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Down:
                                // aux is the other face of the edge piece
                                aux = faces[Facing::Left][2][1];
                                if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "L D L' D'", WCRtotal);
                                    // restart the lookup because the cube changed
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "L D2 L' D2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "L D' L' D", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
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
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "D' F D", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "F", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else // left
                                {
                                    execute(cube, "D F D'", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Left:
                                // rotate cube as the next cases are permutations of case Facing::Right
                                execute(cube, "Y2", WCRtotal);
                                // restart the lookup because the cube changed
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                break;
                            case Facing::Front:
                                execute(cube, "Y'", WCRtotal);
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                break;
                            case Facing::Back:
                                execute(cube, "Y", WCRtotal);
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
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
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "R2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "U F2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else
                                {
                                    execute(cube, "U2 L2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Down:
                                // aux is the other face of the edge piece
                                aux = faces[Facing::Right][2][1];
                                if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "R D' R' D", WCRtotal);
                                    // restart the lookup because the cube changed
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    // the piece is in the right position, lookup continues
                                    c++;
                                    continue;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "R D R' D'", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else // left
                                {
                                    execute(cube, "R D2 R' D2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Right:
                                // aux is the other face of the edge piece
                                aux = faces[Facing::Back][1][0];
                                if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "B'", WCRtotal);
                                    // restart the lookup because the cube changed
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "D B' D'", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "D2 B' D2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else // left
                                {
                                    execute(cube, "D' B' D", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Left:
                                // rotate cube as the next cases are permutations of case Facing::Right
                                execute(cube, "Y2", WCRtotal);
                                // restart the lookup because the cube changed
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                break;
                            case Facing::Front:
                                execute(cube, "Y'", WCRtotal);
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                break;
                            case Facing::Back:
                                execute(cube, "Y", WCRtotal);
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
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
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "U' R2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "F2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else
                                {
                                    execute(cube, "U L2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
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
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "B D2 B' D2", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else // left
                                {
                                    execute(cube, "B D' B' D", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Right:
                                // aux is the other face of the edge piece
                                aux = faces[Facing::Down][1][2];
                                if (aux == faces[Facing::Back][1][1])
                                {
                                    execute(cube, "R' B'", WCRtotal);
                                    // restart the lookup because the cube changed
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Right][1][1])
                                {
                                    execute(cube, "R' D' B' D", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else if (aux == faces[Facing::Front][1][1])
                                {
                                    execute(cube, "R F", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                } else // left
                                {
                                    execute(cube, "R D' F D", WCRtotal);
                                    f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                }
                                break;
                            case Facing::Left:
                                // rotate cube as the next cases are permutations of case Facing::Right
                                execute(cube, "Y2", WCRtotal);
                                // restart the lookup because the cube changed
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                break;
                            case Facing::Front:
                                execute(cube, "Y'", WCRtotal);
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
                                break;
                            case Facing::Back:
                                execute(cube, "Y", WCRtotal);
                                f = 0, i = 0, j = 0, cube.copy(faces), c = 0;
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

    /* Count up moves */
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

void Basic::execute(Cube &cube, std::string sequence, int &ctr)
{
    Parser::parse(cube, sequence, ctr, false, false);
}