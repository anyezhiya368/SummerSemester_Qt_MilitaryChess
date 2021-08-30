#ifndef MAPNODE_H
#define MAPNODE_H

#include <vector>

class MapNode
{
public:
    MapNode(int, int);
    int getx();
    void setx(int);
    int gety();
    void sety(int);
    std::vector<int> neighborx;
    std::vector<int> neighbory;
private:
    int x;
    int y;
};

#endif // MAPNODE_H
