#include "mapnode.h"

MapNode::MapNode(int x, int y)
{
    this->x = x;
    this->y = y;
    if(x == 0 && y == 1)
    {
        neighborx.push_back(1);
        neighbory.push_back(1);
        neighborx.push_back(0);
        neighbory.push_back(2);
    }else if(x >= 1 && x <= 3 && (y == 1 || y == 10))
    {
        neighborx.push_back(x - 1);
        neighbory.push_back(y);
        neighborx.push_back(x + 1);
        neighbory.push_back(y);
    }else if(x == 4 && y == 1)
    {
        neighborx.push_back(3);
        neighbory.push_back(1);
        neighborx.push_back(4);
        neighbory.push_back(2);
    }else if(x == 0 && y == 10)
    {
        neighborx.push_back(0);
        neighbory.push_back(9);
        neighborx.push_back(1);
        neighbory.push_back(10);
    }else if(x == 4 && y == 10)
    {
        neighborx.push_back(3);
        neighbory.push_back(10);
        neighborx.push_back(4);
        neighbory.push_back(9);
    }else if((x == 0 || x == 4) && ((y >= 2 && y <= 4) || (y >= 7 && y <= 9)))
    {
        neighborx.push_back(x);
        neighbory.push_back(y - 1);
        neighborx.push_back(x);
        neighbory.push_back(y + 1);
    }else if((x == 1 && y == 5) || (x == 3 && y == 5) || (x == 1 && y == 6) || (x == 3 && y == 6))
    {
        neighborx.push_back(x - 1);
        neighbory.push_back(y);
        neighborx.push_back(x + 1);
        neighbory.push_back(y);
    }else if((x == 0 && y == 5) || (x == 0 && y == 6))
    {
        neighborx.push_back(x);
        neighbory.push_back(y - 1);
        neighborx.push_back(x);
        neighbory.push_back(y + 1);
        neighborx.push_back(x + 1);
        neighbory.push_back(y);
    }else if((x == 4 && y ==5) || (x == 4 && y == 6))
    {
        neighborx.push_back(x);
        neighbory.push_back(y + 1);
        neighborx.push_back(x);
        neighbory.push_back(y - 1);
        neighborx.push_back(x - 1);
        neighbory.push_back(y);
    }else if(x == 2 && y == 5)
    {
        neighborx.push_back(x - 1);
        neighbory.push_back(y);
        neighborx.push_back(x + 1);
        neighbory.push_back(y);
        neighborx.push_back(x);
        neighbory.push_back(y + 1);
    }else if(x == 2 && y == 6)
    {
        neighborx.push_back(x - 1);
        neighbory.push_back(y);
        neighborx.push_back(x + 1);
        neighbory.push_back(y);
        neighborx.push_back(x);
        neighbory.push_back(y - 1);
    }else
    {
        neighborx.push_back(-1);
        neighbory.push_back(-1);
    }
}

int MapNode::getx()
{
    return x;
}

void MapNode::setx(int value)
{
    x = value;
}

int MapNode::gety()
{
    return y;
}

void MapNode::sety(int value)
{
    y = value;
}
