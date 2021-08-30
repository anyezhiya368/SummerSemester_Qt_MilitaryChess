#include "checkboard.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

CheckBoard::CheckBoard()
{
    x[0] = (int) 368 / 1105.0 * 110.0;
    x[1] = (int) 368 / 1105.0 * 330.0;
    x[2] = (int) 368 / 1105.0 * 550.0;
    x[3] = (int) 368 / 1105.0 * 770.0;
    x[4] = (int) 368 / 1105.0 * 990.0;
    y[0] = (int) 512 / 1536.0 * 67.0;
    y[1] = (int) 512 / 1536.0 * 170.0;
    y[2] = (int) 512 / 1536.0 * 277.0;
    y[3] = (int) 512 / 1536.0 * 387.0;
    y[4] = (int) 512 / 1536.0 * 495.0;
    y[5] = (int) 512 / 1536.0 * 605.0;
    y[6] = (int) 512 / 1536.0 * 936.0;
    y[7] = (int) 512 / 1536.0 * 1046.0;
    y[8] = (int) 512 / 1536.0 * 1152.0;
    y[9] = (int) 512 / 1536.0 * 1260.0;
    y[10] = (int) 512 / 1536.0 * 1367.0;
    y[11] = (int) 512 / 1536.0 * 1476.0;
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
            node[i][j] = -3;

    node[1][2] = -1;
    node[3][2] = -1;
    node[2][3] = -1;
    node[1][4] = -1;
    node[3][4] = -1;
    node[1][7] = -1;
    node[3][7] = -1;
    node[2][8] = -1;
    node[1][9] = -1;
    node[3][9] = -1;

    seed = time(0);

    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
            init[i][j] = -1;
    int number[50];
    for(int i = 0; i < 50; i++)
        number[i] = i;
    srand((unsigned)seed);
    std::random_shuffle(number, number + 50);
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 2; j++)
            init[i][j] = number[j * 5 + i];
    init[0][2] = number[10];
    init[2][2] = number[11];
    init[4][2] = number[12];
    init[0][3] = number[13];
    init[1][3] = number[14];
    init[3][3] = number[15];
    init[4][3] = number[16];
    init[0][4] = number[17];
    init[2][4] = number[18];
    init[4][4] = number[19];
    for(int i = 0; i < 5; i++)
        init[i][5] = number[5 * 5 + i - 5];
    for(int i = 0; i < 5; i++)
        init[i][6] = number[5 * 6 + i - 5];
    init[0][7] = number[30];
    init[2][7] = number[31];
    init[4][7] = number[32];
    init[0][8] = number[33];
    init[1][8] = number[34];
    init[3][8] = number[35];
    init[4][8] = number[36];
    init[0][9] = number[37];
    init[2][9] = number[38];
    init[4][9] = number[39];
    for(int i = 0; i < 5; i++)
        for(int j = 10; j < 12; j++)
            init[i][j] = number[j * 5 + i - 10];

}
int CheckBoard::getNode(int x, int y)
{
    return node[x][y];
}

void CheckBoard::setNode(int x, int y, int val)
{
    node[x][y] = val;
}

int CheckBoard::getPlayer()
{
    return player;
}

void CheckBoard::setPlayer(int id)
{
    player = id;
}

int CheckBoard::getinit(int x, int y)
{
    return init[x][y];
}

void CheckBoard::setinit(int x, int y, int value)
{
    init[x][y] = value;
}

int CheckBoard::getseed()
{
    return seed;
}

void CheckBoard::setseed(int value)
{
    seed = value;
}

int CheckBoard::original(int axisx, int axisy)
{
    if((axisx == 1 && axisy == 2) ||
       (axisx == 3 && axisy == 2) ||
       (axisx == 2 && axisy == 3) ||
       (axisx == 1 && axisy == 4) ||
       (axisx == 3 && axisy == 4) ||
       (axisx == 1 && axisy == 7) ||
       (axisx == 3 && axisy == 7) ||
       (axisx == 2 && axisy == 8) ||
       (axisx == 1 && axisy == 9) ||
       (axisx == 3 && axisy == 9)
            )
        return -1;
    else
        return -2;
}

bool CheckBoard::remainlandmine(int side)
{
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
        {
            if(node[i][j] == -3 && landmine(side, init[i][j]))
                return true;
            if(landmine(side, node[i][j]))
                return true;
        }
    return false;
}

bool CheckBoard::landmine(int side, int state)
{
    if(side == 0)
    {
        if(state == 2 || state == 4 || state == 6)
            return true;
        else
            return false;
    }
    else
    {
        if(state == 3 || state == 5 || state == 7)
            return true;
        else
            return false;
    }
}

void CheckBoard::changeplayer()
{
    if(player == 0)
        player = 1;
    if(player == 1)
        player = 0;
}
