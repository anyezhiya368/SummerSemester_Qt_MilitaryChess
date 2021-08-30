#ifndef CHESSNODE_H
#define CHESSNODE_H

#include <QString>
#include <QPixmap>

class ChessNode
{
public:
    ChessNode(int);
    ~ChessNode();
    int getPlayer();
    void setPlayer(int);
    int getPosition();
    void setPosition(int);
    int getPositionX();
    void setPositionX(int);
    int getPositionY();
    void setPositionY(int);
    int getid();
    void setid(int);
    QString getname();
    void setname(QString);
    QPixmap geticon();
    void seticon(QPixmap);
    QString getpath();
    void setpath(QString);
private:
    QString name;
    QString path;
    QPixmap icon;
    int id;
    int nodewidth;
    int nodeheight;
    int player; // 1 for red and 0 for blue -1 for vacant and questionmark
    int position;
    int positionx;
    int positiony;
};

#endif // CHESSNODE_H
