#include "chessnode.h"

ChessNode::ChessNode(int initialization): id{initialization}
{
    nodewidth = (int) 368 / 1105.0 * 188.0;
    nodeheight = (int) 512 / 1536.0 * 94.0;

    switch (id) {
    case -3:
        name = "questionmark";
        icon = QPixmap(":/new/prefix1/questionmark.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = -1;
        id = -3;
        break;
    case -2:
        name = "vacantstation";
        icon = QPixmap(":/new/prefix1/vacantstation.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = -1;
        id = -2;
        break;
    case -1:
        name = "vacantbase";
        icon = QPixmap(":/new/prefix1/vacantbase.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = -1;
        id = -1;
        break;
    case 0:
        name = "bannerblue";
        icon = QPixmap(":/new/prefix1/bannerblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 0;
        break;
    case 1:
        name = "bannerred";
        icon = QPixmap(":/new/prefix1/bannerred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 1;
        id = 1;
        break;
    case 2:
        name = "landmineblue";
        icon = QPixmap(":/new/prefix1/landmineblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 2;
        break;
    case 3:
        name = "landminered";
        icon = QPixmap(":/new/prefix1/landminered.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 1;
        id = 3;
        break;
    case 4:
        name = "landmineblue";
        icon = QPixmap(":/new/prefix1/landmineblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 4;
        break;
    case 5:
        name = "landminered";
        icon = QPixmap(":/new/prefix1/landminered.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 5;
        break;
    case 6:
        name = "landmineblue";
        icon = QPixmap(":/new/prefix1/landmineblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 6;
        break;
    case 7:
        name = "landminered";
        icon = QPixmap(":/new/prefix1/landminered.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 7;
        break;
    case 8:
        name = "bomberblue";
        icon = QPixmap(":/new/prefix1/bomberblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 8;
        break;
    case 9:
        name = "bomberred";
        icon = QPixmap(":/new/prefix1/bomberred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 9;
        break;
    case 10:
        name = "bomberblue";
        icon = QPixmap(":/new/prefix1/bomberblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 10;
        break;
    case 11:
        name = "bomberred";
        icon = QPixmap(":/new/prefix1/bomberred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 11;
        break;
    case 12:
        name = "sapperblue";
        icon = QPixmap(":/new/prefix1/sapperblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 12;
        break;
    case 13:
        name = "sapperred";
        icon = QPixmap(":/new/prefix1/sapperred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 13;
        break;
    case 14:
        name = "sapperblue";
        icon = QPixmap(":/new/prefix1/sapperblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 14;
        break;
    case 15:
        name = "sapperred";
        icon = QPixmap(":/new/prefix1/sapperred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 15;
        break;
    case 16:
        name = "sapperblue";
        icon = QPixmap(":/new/prefix1/sapperblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 16;
        break;
    case 17:
        name = "sapperred";
        icon = QPixmap(":/new/prefix1/sapperred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 17;
        break;
    case 18:
        name = "platoonblue";
        icon = QPixmap(":/new/prefix1/platoonblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 18;
        break;
    case 19:
        name = "platoonred";
        icon = QPixmap(":/new/prefix1/platoonred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 19;
        break;
    case 20:
        name = "platoonblue";
        icon = QPixmap(":/new/prefix1/platoonblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 20;
        break;
    case 21:
        name = "platoonred";
        icon = QPixmap(":/new/prefix1/platoonred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 21;
        break;
    case 22:
        name = "platoonblue";
        icon = QPixmap(":/new/prefix1/platoonblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 22;
        break;
    case 23:
        name = "platoonred";
        icon = QPixmap(":/new/prefix1/platoonred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 23;
        break;
    case 24:
        name = "captainblue";
        icon = QPixmap(":/new/prefix1/captainblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 24;
        break;
    case 25:
        name = "captainred";
        icon = QPixmap(":/new/prefix1/captainred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 25;
        break;
    case 26:
        name = "captainblue";
        icon = QPixmap(":/new/prefix1/captainblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 26;
        break;
    case 27:
        name = "captainred";
        icon = QPixmap(":/new/prefix1/captainred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 27;
        break;
    case 28:
        name = "captainblue";
        icon = QPixmap(":/new/prefix1/captainblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 28;
        break;
    case 29:
        name = "captainred";
        icon = QPixmap(":/new/prefix1/captainred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 29;
        break;
    case 30:
        name = "battalionblue";
        icon = QPixmap(":/new/prefix1/battalionblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 30;
        break;
    case 31:
        name = "battalionred";
        icon = QPixmap(":/new/prefix1/battalionred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 31;
        break;
    case 32:
        name = "battalionblue";
        icon = QPixmap(":/new/prefix1/battalionblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 32;
        break;
    case 33:
        name = "battalionred";
        icon = QPixmap(":/new/prefix1/battalionred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 33;
        break;
    case 34:
        name = "colonelblue";
        icon = QPixmap(":/new/prefix1/colonelblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 34;
        break;
    case 35:
        name = "colonelred";
        icon = QPixmap(":/new/prefix1/colonelred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 35;
        break;
    case 36:
        name = "colonelblue";
        icon = QPixmap(":/new/prefix1/colonelblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 36;
        break;
    case 37:
        name = "colonelred";
        icon = QPixmap(":/new/prefix1/colonelred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 37;
        break;
    case 38:
        name = "brigadeblue";
        icon = QPixmap(":/new/prefix1/brigadeblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 38;
        break;
    case 39:
        name = "brigadered";
        icon = QPixmap(":/new/prefix1/brigadered.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 39;
        break;
    case 40:
        name = "brigadeblue";
        icon = QPixmap(":/new/prefix1/brigadeblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 40;
        break;
    case 41:
        name = "brigadered";
        icon = QPixmap(":/new/prefix1/brigadered.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 41;
        break;
    case 42:
        name = "teacherblue";
        icon = QPixmap(":/new/prefix1/teacherblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 42;
        break;
    case 43:
        name = "teacherred";
        icon = QPixmap(":/new/prefix1/teacherred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 43;
        break;
    case 44:
        name = "teacherblue";
        icon = QPixmap(":/new/prefix1/teacherblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 44;
        break;
    case 45:
        name = "teacherred";
        icon = QPixmap(":/new/prefix1/teacherred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 45;
        break;
    case 46:
        name = "generalblue";
        icon = QPixmap(":/new/prefix1/generalblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 46;
        break;
    case 47:
        name = "generalred";
        icon = QPixmap(":/new/prefix1/generalred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 47;
        break;
    case 48:
        name = "commanderblue";
        icon = QPixmap(":/new/prefix1/commanderblue.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 48;
        break;
    case 49:
        name = "commanderred";
        icon = QPixmap(":/new/prefix1/commanderred.png");
        icon = icon.scaled(nodewidth, nodeheight);
        player = 0;
        id = 49;
        break;
    }
}

int ChessNode::getPlayer()
{
    return player;
}

void ChessNode::setPlayer(int id)
{
    player = id;
}

int ChessNode::getPosition()
{
    return position;
}

void ChessNode::setPosition(int pos)
{
    position = pos;
}

int ChessNode::getPositionX()
{
    return positionx;
}

void ChessNode::setPositionX(int pos)
{
    positionx = pos;
}

int ChessNode::getPositionY()
{
    return positiony;
}

void ChessNode::setPositionY(int pos)
{
    positiony = pos;
}

int ChessNode::getid()
{
    return id;
}

void ChessNode::setid(int value)
{
    id = value;
}

QPixmap ChessNode::geticon()
{
    return icon;
}

void ChessNode::seticon(QPixmap pict)
{
    icon = pict;
}

QString ChessNode::getname()
{
    return name;
}

void ChessNode::setname(QString value)
{
    name = value;
}

QString ChessNode::getpath()
{
    return path;
}

void ChessNode::setpath(QString str)
{
    path = str;
}

ChessNode::~ChessNode()
{

}
