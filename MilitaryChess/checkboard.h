#ifndef CHECKBOARD_H
#define CHECKBOARD_H

#include <QDebug>
#include "chessnode.h"

class CheckBoard
{
public:
    CheckBoard();
    ~CheckBoard() {}
    int getNode(int, int);
    void setNode(int, int, int);
    int getPlayer();
    void setPlayer(int);
    int getinit(int, int);
    void setinit(int, int, int);
    int getseed();
    void setseed(int);
    int original(int, int);
    bool remainlandmine(int);
    bool landmine(int, int);
    void changeplayer();
    int x[5];
    int y[12];
private:
    int init[5][12]; // store the initial chessnodes arragement, bases are -1
    int node[5][12]; // containing the current chessnodes state
    int player; // 0 for you and 1 for other
    int seed; // random seed
};

#endif // CHECKBOARD_H
