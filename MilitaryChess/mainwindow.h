#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QString>
#include <QPixmap>
#include <QGroupBox>
#include <QTimer>
#include <vector>
#include <QThread>
#include <QMessageBox>
#include <stack>
#include "checkboard.h"
#include "chessnode.h"
#include "ipset.h"
#include "ipvalidation.h"
#include "mapnode.h"
#include "myserver.h"
#include "mysocket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void notstarted();
    void notleft();
    void mouse(int, int);
    int transferx(int);
    int transfery(int);
    void initlabels();
    void display(int, int, int);
    void updatetime();
    void updatecollision(int, int, ChessNode*, int, int, ChessNode*);
    bool lawful(int, int, ChessNode*, int, int, ChessNode*);
    bool special(int, int);
    bool reachable(int, int, int, int, int);
    void playerwarn();
    void playerwarnsecond();
    bool bigger(ChessNode*, ChessNode*);
    bool equal(ChessNode*, ChessNode*);
    std::vector<MapNode*> generateproposals(int, int, int);
    void dfs(int, int, std::vector<MapNode*>&);
    bool judgedefeat(); // a prepared function which will be used after the received network signal is updated
    void sendnetworksignal(char*);
    void decode(char*);
    void initmessagewrite();
    void dealwithconnection();
    void dealipsetsignal(QString);
    void dealipvalsignal();
    void dealstart();
    void dealinitializationsignal();
    void settimerlabel();
    void dealdefeat();
    void deallosesignal();
    void dealwinsignal();
    bool alldiscovered();
    void dealdisconnect();
signals:
    void listensignal(bool, bool);
    void writesignal(bool, QString, char*);
    void initializationsignal();
    void losesignal();
    void winsignal();
protected:
    void mousePressEvent(QMouseEvent* ev);
private:
    Ui::MainWindow *ui;
    QDockWidget* dock;
    QPainter* painter;
    bool started; // whether the game has started
    IPSet* ipset;
    IPValidation* ipvalidation;
    QLabel* timerlabel;
    QTimer* timer;
    QLabel* maplabel;
    bool auselessflag;
    QLabel* mylabels[5][12];
    CheckBoard* checkboard;
    bool firstclick;
    bool secondclick;
    ChessNode* firstnode;
    ChessNode* secondnode;
    int firstclickx;
    int firstclicky;
    bool searched[5][12];
    QThread* mythread;
    MyServer* myserver;
    MySocket* mysocket;
    QString connectip;
    QString hostip;
    bool otherstarted;
    char messagewrite[64];
    bool firstplayer; // mark whether you are the one who send the invitation
    int initplayer;
    int playertimes;
    int yourplayer; // -1 for not decide, 0 for blue and 1 for red
    QString yourside;
    QString currentplayer;
    bool chosenplayer;
    std::stack<int> lastcolors;
    int time;
    int outtimes[2];
};
#endif // MAINWINDOW_H
