#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QDebug>
#include <QTextEdit>
#include <QAction>
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Happy Military Chess!");
    this->setFixedSize(560, 640);


    started = false;

    dock = new QDockWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    QList<QDockWidget*> dockList;
    QList<int> intList;
    dockList.append(dock);
    intList.append(100);
    resizeDocks(dockList, intList, Qt::Horizontal);

    QTextEdit* tEdit = new QTextEdit(this);
    tEdit->setReadOnly(true);
    tEdit->setText("军棋的棋子每方各有25个，分别为军旗、司令、军长各一；师长、旅长、团长、营长、炸弹各二；连长、排长、工兵、地雷各三。\n\n司令>军长>师长>旅长>团长>营长>连长>排长>工兵；小棋遇大棋被吃；相同棋子相遇，则同归于尽；工兵与炸弹能排除地雷，其它棋子不能排雷，不可排雷的棋子碰到地雷会被吃；任何棋子都能吃掉军旗；炸弹与任何棋子相遇时同归于尽。");
    dock->setWidget(tEdit);

    timerlabel = new QLabel(this);
    timerlabel->setFixedSize(368, 64);
    timerlabel->move(46, 10);
    timerlabel->hide();
    timer = new QTimer(this);
    time = 20;

    timerlabel->hide();

    connect(timer, &QTimer::timeout, this, &MainWindow::updatetime);

    auselessflag = false;

    checkboard = new CheckBoard();

    maplabel = new QLabel(this);
    maplabel->setFixedSize(368, 512);
    maplabel->move(46, 64);
    QPixmap image = QPixmap("../Images/png/chessboard.png");
    image = image.scaled(maplabel->size());
    maplabel->setPixmap(image);
    maplabel->hide();

    int w = (int) 368 / 1105.0 * 188.0;;
    int h = (int) 512 / 1536.0 * 94.0;
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
        {
            mylabels[i][j] = new QLabel(maplabel);
            mylabels[i][j]->setFixedSize(w, h);
            mylabels[i][j]->move(checkboard->x[i] - w / 2, checkboard->y[j] - h / 2);
            //QPixmap image = QPixmap(":/new/prefix1/bannerblue.png");
            //image = image.scaled(mylabels[i][j]->size());
            //mylabels[i][j]->setPixmap(image);
            //mylabels[i][j]->show();
            //mylabels[i][j]->setVisible(false);
        }
    ChessNode* vacantbase = new ChessNode(-1);
    QPixmap imagevacantbase= vacantbase->geticon();
    mylabels[1][2] -> setPixmap(imagevacantbase);
    mylabels[1][2]->show();
    mylabels[3][2] -> setPixmap(imagevacantbase);
    mylabels[3][2]->show();
    mylabels[2][3] -> setPixmap(imagevacantbase);
    mylabels[2][3]->show();
    mylabels[1][4] -> setPixmap(imagevacantbase);
    mylabels[1][4]->show();
    mylabels[3][4] -> setPixmap(imagevacantbase);
    mylabels[3][4]->show();
    mylabels[1][7] -> setPixmap(imagevacantbase);
    mylabels[1][7]->show();
    mylabels[3][7] -> setPixmap(imagevacantbase);
    mylabels[3][7]->show();
    mylabels[2][8] -> setPixmap(imagevacantbase);
    mylabels[2][8]->show();
    mylabels[1][9] -> setPixmap(imagevacantbase);
    mylabels[1][9]->show();
    mylabels[3][9] -> setPixmap(imagevacantbase);
    mylabels[3][9]->show();

    firstclick = true;
    secondclick = false;

    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
            searched[i][j] = false;


    hostip = "";
    connectip = "";

    QString hostname = QHostInfo::localHostName();
    QHostInfo hostinfo = QHostInfo::fromName(hostname);
    auto IPList = hostinfo.addresses();
    foreach(auto ipaddress, IPList)
    {
        if((ipaddress.protocol() == QAbstractSocket::IPv4Protocol) && (ipaddress != QHostAddress::LocalHost))
        {
            hostip += ipaddress.toString();
            break;
        }
    }

    playertimes = 0;
    initplayer = rand() % 2;
    outtimes[0] = 0;
    outtimes[1] = 0;
    yourplayer = -1;

    otherstarted = false;
    firstplayer = false;
    chosenplayer = false;

    ipset = new IPSet();
    ipvalidation = new IPValidation();

    mythread = new QThread(this);
    myserver = new MyServer();
    mysocket = new MySocket();

    connect(ui->actionCreate_the_connection, &QAction::triggered,
            [=]()
            {
                ipvalidation->showip();
                emit listensignal(false, firstplayer);
            });

    connect(ipset, &IPSet::MySignal, this, &MainWindow::dealipsetsignal);


    connect(ui->actionConnect_to_server, &QAction::triggered,
            [=]()
            {
                ipset->show();
            }
            );

    connect(ui->actionAdmit_defeat, &QAction::triggered, this, &MainWindow::dealdefeat);
    connect(ipvalidation, &IPValidation::IPValSignal, this, &MainWindow::dealipvalsignal);
    connect(ui->actionStart, &QAction::triggered, this, &MainWindow::dealstart);
    connect(this, &MainWindow::initializationsignal, this, &MainWindow::dealinitializationsignal);
    connect(ui->actionDisable_the_connection, &QAction::triggered, this, &MainWindow::dealdisconnect);


    myserver->moveToThread(mythread);
    mysocket->moveToThread(mythread);

    mythread->start();

    connect(this, &MainWindow::writesignal, mysocket, &MySocket::Write);
    connect(this, &MainWindow::listensignal, myserver, &MyServer::Listen);
    //connect(mysocket, &MySocket::finishwriting, this, &MainWindow::dealfinishwritingsignal);
    connect(myserver, &MyServer::finishreading, this, &MainWindow::decode);

    connect(this, &MainWindow::losesignal, this, &MainWindow::deallosesignal);
    connect(this, &MainWindow::winsignal, this, &MainWindow::dealwinsignal);

    //qDebug()<<QThread::currentThread();

}

void MainWindow::decode(char* message)
{
    qDebug()<<message;
    //QMessageBox::information(this, "", "We have received a piece of information");
    // decode the initialization message
    if(message[0] == 'a')
    {
       //QMessageBox::information(this, "", "Received a message");
       int length = message[1] - '0';
       connectip = "";
       for(int i = 0; i < length; i++)
           connectip += message[2 + i];
    }
    // decode the vacant message
    if(message[0] == 'b')
    {
        //QMessageBox::information(this, "", "Received b message");
        otherstarted = true;
        QMessageBox::information(this, "", "The other player has started.");
    }
    // decode the reject connection message
    if(message[0] == 'c')
    {
        //QMessageBox::information(this, "", "Received c message");
        started = true;
        ui->actionStart->setEnabled(false);
        emit initializationsignal();
        QMessageBox::information(this, "", "The game has started.");
    }
    // decode the chessboard initialization
    if(message[0] == 'd')
    {
        for(int i = 0; i < 60; i++)
        {
            if(message[i + 1] != '[')
            {
                char received = message[i + 1];
                int value = received - 33;
                checkboard->setinit(i % 5, i / 5, value);
            }
            else
                checkboard->setinit(i % 5, i / 5, -1);
        }
        if(message[61] == (0 + '0'))
        {
            checkboard->setPlayer(1);
        }
        else
        {
            checkboard->setPlayer(0);
        }
        time = 20;
        timerlabel->show();
        timer->start(1000);
    }
    if(message[0] == '1')
    {
        if(message[1] == '0')
        {
            time = 20;
            int axisx = message[2] - 33;
            int axisy = message[3] - 33;
            int nodetoget = checkboard->getinit(axisx, axisy);
            display(axisx, axisy, nodetoget);
            checkboard->setNode(axisx, axisy, nodetoget);
            playertimes ++;
            checkboard->setPlayer(0);
        }else
        {
            time = 20;
            int othercolor = message[2] - '0';
            int axisx = message[3] - 33;
            int axisy = message[4] - 33;
            int nodetoget = checkboard->getinit(axisx, axisy);
            display(axisx, axisy, nodetoget);
            checkboard->setNode(axisx, axisy, nodetoget);
            playertimes ++;
            checkboard->setPlayer(0);
            if(othercolor == 0)
                yourplayer = 1;
            else
                yourplayer = 0;
            chosenplayer = true;
        }
    }
    if(message[0] == '2')
    {
        int axisx = message[1] - 33;
        int axisy = message[2] - 33;
        int nodetoget = checkboard->getinit(axisx, axisy);
        display(axisx, axisy, nodetoget);
        checkboard->setNode(axisx, axisy, nodetoget);
        playertimes ++;
        time = 20;
        checkboard->setPlayer(0);
    }

    if(message[0] == '3')
    {
        int axisx = message[1] - 33;
        int axisy = message[2] - 33;
        mylabels[axisx][axisy]->setStyleSheet("QLabel{border:1px solid rgb(0, 255, 0);}");
    }
    if(message[0] == '4')
    {
        int axisx = message[1] - 33;
        int axisy = message[2] - 33;
        mylabels[axisx][axisy]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
    }

    if(message[0] == '5')
    {
        int firstx = message[1] - 33;
        int firsty = message[2] - 33;
        int secondx = message[3] - 33;
        int secondy = message[4] - 33;
        mylabels[firstx][firsty]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
        display(firstx, firsty, checkboard->original(firstx, firsty));
        display(secondx, secondy, checkboard->getNode(firstx, firsty));
        checkboard->setNode(secondx, secondy, checkboard->getNode(firstx, firsty));
        checkboard->setNode(firstx, firsty, checkboard->original(firstx, firsty));
        playertimes ++;
        time = 20;
        checkboard->setPlayer(0);
    }
    if(message[0] == '6')
    {
        int firstx = message[1] - 33;
        int firsty = message[2] - 33;
        ChessNode* nodefirst = new ChessNode(checkboard->getNode(firstx, firsty));
        int secondx = message[3] - 33;
        int secondy = message[4] - 33;
        ChessNode* nodesecond = new ChessNode(checkboard->getNode(secondx, secondy));
        mylabels[firstx][firsty]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
        updatecollision(firstx, firsty, nodefirst, secondx, secondy, nodesecond);
        playertimes ++;
        time = 20;
        checkboard->setPlayer(0);
    }
    if(message[0] == 'l')
    {
        timer->stop();
        QMessageBox::information(this, "", "You have won the game!");
    }
    if(message[0] == 'w')
    {
        timer->stop();
        QMessageBox::information(this, "", "You have lost the game!");
    }
    if(message[0] == 'z')
    {
        emit listensignal(true, firstplayer);
        QMessageBox::information(this, "", "The other side disconnect!");
    }
    if(alldiscovered() && judgedefeat())
    {
        /*
        for(int i = 0; i < 5; i++)
            for(int j = 0; j < 12; j++)
                qDebug()<<checkboard->getNode(i, j);*/
        emit losesignal();
        //QMessageBox::information(this, "", "You have lost the game for nowhere to go");
    }
}

void MainWindow::dealinitializationsignal()
{
    if(initplayer == 0)
    {
        checkboard->setPlayer(0);
    }
    else
    {
        checkboard->setPlayer(1);
    }
    initmessagewrite();
    messagewrite[0] = 'd';
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
        {
            if(checkboard->getinit(i, j) != -1)
                messagewrite[j * 5 + i + 1] = checkboard->getinit(i, j) + 33;
            else
                messagewrite[j * 5 + i + 1] = '[';
        }
    messagewrite[61] = initplayer + '0';
    emit writesignal(firstplayer, connectip, messagewrite);
    time = 20;
    timerlabel->show();
    timer->start(1000);
}

void MainWindow::dealstart()
{
    if(!otherstarted)
    {
        initmessagewrite();
        messagewrite[0] = 'b';
        emit writesignal(firstplayer, connectip, messagewrite);
        QMessageBox::information(this, "", "The other player has not started.");
        //QMessageBox::information(this, "", "Sent b message");
    }else
    {
        started = true;
        ui->actionStart->setEnabled(false);
        initmessagewrite();
        messagewrite[0] = 'c';
        emit writesignal(firstplayer, connectip, messagewrite);
        //QMessageBox::information(this, "", "Sent c message");
    }
}


void MainWindow::dealipvalsignal()
{
    if(firstplayer)
    {
        maplabel->show();
        int iplength = hostip.length();
        initmessagewrite();
        messagewrite[0] = 'a';
        messagewrite[1] = iplength + '0';
        for(int i = 0; i < iplength; i++)
        {
            if(hostip.mid(i, 1) != '.')
                messagewrite[i + 2] = hostip.mid(i, 1).toInt() + '0';
            else
                messagewrite[i + 2] = '.';
        }

        emit writesignal(firstplayer, connectip, messagewrite);
        //QMessageBox::information(this, "", "Sent a message");
    }else
        maplabel->show();
}


void MainWindow::dealipsetsignal(QString ipsent)
{
    firstplayer = true;
    connectip += ipsent;
}

void MainWindow::dealdefeat()
{
    if(playertimes < 20)
    {
        QMessageBox::warning(this, "", "The total times have not reached 20.");
    }else if(checkboard->getPlayer() == 1)
    {
        QMessageBox::warning(this, "", "It is not your turn now.");
    }
    else
    {
        emit losesignal();
    }
}


void MainWindow::deallosesignal()
{
    initmessagewrite();
    messagewrite[0] = 'l';
    emit writesignal(firstplayer, connectip, messagewrite);
    timer->stop();
    QMessageBox::information(this, "", "You have lost the game!");
}

void MainWindow::dealwinsignal()
{
    initmessagewrite();
    messagewrite[0] = 'w';
    emit writesignal(firstplayer, connectip, messagewrite);
    timer->stop();
    QMessageBox::information(this, "", "You have won the game!");
}

void MainWindow::dealdisconnect()
{
    initmessagewrite();
    messagewrite[0] = 'z';
    emit writesignal(firstplayer, connectip, messagewrite);
}

void MainWindow::updatetime()
{
//When the pogram has received or sent a network signal, the slot function should include reset the time as 20
//also, the slot function should include changing the player variable in the checkboard class
//Important!!! When sending the network signal, the fistclick and secondclick should be reset!!!
    if(time > 0)
        time -= 1;
    else
    {
        if(outtimes[0] < 2)
        {
           if(firstclick)
           {
               if(checkboard->getPlayer() == 0)
               {
                   outtimes[0] ++;
                   checkboard->setPlayer(1);
                   time = 20;
               }else
               {
                   outtimes[1]++;
                   checkboard->setPlayer(0);
                   time = 20;
               }
           }else
           {
               if(checkboard->getPlayer() == 0)
               {
                   outtimes[0] ++;
                   checkboard->setPlayer(1);
                   time = 20;
                   mylabels[firstclickx][firstclicky]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
                   std::vector<MapNode*> proposals = generateproposals(firstclickx, firstclicky, checkboard->getNode(firstclickx, firstclicky));
                   for(int i = 0; i < proposals.size(); i++)
                   {
                       int proposalx = proposals[i]->getx();
                       int proposaly = proposals[i]->gety();
                       mylabels[proposalx][proposaly]->setStyleSheet("QLabel{border:0px solid rgb(0, 120, 120);}");
                   }
                   messagewrite[0] = '4';
                   messagewrite[1] = firstclickx + 33;
                   messagewrite[2] = firstclicky + 33;
                   emit writesignal(firstplayer, connectip, messagewrite);
               }else
               {
                   outtimes[1] ++;
                   checkboard->setPlayer(0);
                   time = 20;
               }
           }
        }else
        {
            emit losesignal();
        }
    }
    if(checkboard->getPlayer() == 0)
        currentplayer = "you";
    else
        currentplayer = "other";
    if(yourplayer == -1)
        yourside = "not decided";
    else if(yourplayer == 0)
        yourside = "blue";
    else
        yourside = "red";
    settimerlabel();
}

void MainWindow::notstarted()
{
    QMessageBox::warning(this, "", "Error! The game has not started.");
}

void MainWindow::notleft()
{
    QMessageBox::warning(this, "", "Error! Please click on the left button.");
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(!started)
        this->notstarted();
    else if(ev->button() != Qt::LeftButton)
        this->notleft();
    else
    {
        int mousex = ev->x();
        int mousey = ev->y();
        //qDebug()<<transferx(mousex)<<' '<<transfery(mousey);
        mouse(mousex, mousey);
        //qDebug()<<checkboard->getNode(transferx(mousex), transfery(mousey));
    }
    //qDebug()<<checkboard->getPlayer();
}

int MainWindow::transferx(int mouseX)
{
    return (mouseX - 46) / 73;
}

int MainWindow::transfery(int mouseY)
{
    if(mouseY >= 64 && mouseY <= 576)
    {
        int line1 = 246;
        int line2 = 392;
        int midline = 319;
        if(mouseY < 246 && mouseY >= 64)
            return (mouseY - 64) / 36;
        if(mouseY >= 392 && mouseY <= 572)
            return (mouseY - 392) / 36 + 7;
        if(mouseY >= 246 && mouseY < 319)
            return 5;
        if(mouseY >=319 && mouseY < 392)
            return 6;
    }
}


void MainWindow::mouse(int mouseX, int mouseY)
{
    int axisx = transferx(mouseX);
    int axisy = transfery(mouseY);
    if(chosenplayer)
    {
    if(checkboard->getPlayer() == 0)
    {
    if(checkboard->getNode(axisx, axisy) == -3 && firstclick)
    {
        int nodetoget = checkboard->getinit(axisx, axisy);
        //qDebug() << nodetoget;
        display(axisx, axisy, nodetoget);
        checkboard->setNode(axisx, axisy, nodetoget);
        // Send the network signal
        initmessagewrite();
        messagewrite[0] = '2';
        messagewrite[1] = axisx + 33;
        messagewrite[2] = axisy + 33;
        emit writesignal(firstplayer, connectip, messagewrite);
        // playertimes ++
        playertimes++;
        // reset the time
        time = 20;
        // change the player
        checkboard->setPlayer(1);
    }else if((checkboard->getNode(axisx, axisy) >= -2 && (checkboard->getNode(axisx, axisy) <= 7)) && firstclick)
    {
        playerwarn();
    }else if(checkboard->getNode(axisx, axisy) >= 8 && firstclick && ((checkboard->getNode(axisx, axisy) % 2) == yourplayer))
    {
        int selected = checkboard->getNode(axisx, axisy);
        firstnode = new ChessNode(selected);
        mylabels[axisx][axisy]->setStyleSheet("QLabel{border:1px solid rgb(0, 255, 0);}");
        //(optional) provide all of the possible ways for firstnode
        firstclickx = axisx;
        firstclicky = axisy;
        firstclick = false;
        secondclick = true;
        // emit the network signal

        //
        std::vector<MapNode*> proposals = generateproposals(axisx, axisy, selected);
        for(int i = 0; i < proposals.size(); i++)
        {
            int proposalx = proposals[i]->getx();
            int proposaly = proposals[i]->gety();
            mylabels[proposalx][proposaly]->setStyleSheet("QLabel{border:1px solid rgb(0, 255, 0);}");
        }

        initmessagewrite();
        messagewrite[0] = '3';
        messagewrite[1] = axisx + 33;
        messagewrite[2] = axisy + 33;
        emit writesignal(firstplayer, connectip, messagewrite);

    }else if(checkboard->getNode(axisx, axisy) >= 8 && firstclick && ((checkboard->getNode(axisx, axisy) % 2) != yourplayer))
    {
        playerwarn();
    }
    else if(checkboard->getNode(axisx, axisy) == -3 && secondclick)
    {
        playerwarnsecond();
        firstclick = true;
        secondclick = false;
        mylabels[firstclickx][firstclicky]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
        std::vector<MapNode*> proposals = generateproposals(firstclickx, firstclicky, checkboard->getNode(firstclickx, firstclicky));
        for(int i = 0; i < proposals.size(); i++)
        {
            int proposalx = proposals[i]->getx();
            int proposaly = proposals[i]->gety();
            mylabels[proposalx][proposaly]->setStyleSheet("QLabel{border:0px solid rgb(0, 120, 120);}");
        }
        // emit the network signal

        initmessagewrite();
        messagewrite[0] = '4';
        messagewrite[1] = firstclickx + 33;
        messagewrite[2] = firstclicky + 33;
        emit writesignal(firstplayer, connectip, messagewrite);

    }else if((checkboard->getNode(axisx, axisy) == -2 || checkboard->getNode(axisx, axisy) == -1) && secondclick)
    {
        int selected = checkboard->getNode(axisx, axisy);
        secondnode = new ChessNode(selected);
        if(lawful(firstclickx, firstclicky, firstnode, axisx, axisy, secondnode))
        {
            std::vector<MapNode*> proposals = generateproposals(firstclickx, firstclicky, checkboard->getNode(firstclickx, firstclicky));
            for(int i = 0; i < proposals.size(); i++)
            {
                int proposalx = proposals[i]->getx();
                int proposaly = proposals[i]->gety();
                mylabels[proposalx][proposaly]->setStyleSheet("QLabel{border:0px solid rgb(255, 255, 255);}");
            }
            display(axisx, axisy, firstnode->getid());
            display(firstclickx, firstclicky, checkboard->original(firstclickx, firstclicky));
            checkboard->setNode(axisx, axisy, firstnode->getid());
            checkboard->setNode(firstclickx, firstclicky, checkboard->original(firstclickx, firstclicky));
            firstclick = true;
            secondclick = false;
            mylabels[firstclickx][firstclicky]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
            // Send the network signal
            initmessagewrite();
            messagewrite[0] = '5';
            messagewrite[1] = firstclickx + 33;
            messagewrite[2] = firstclicky + 33;
            messagewrite[3] = axisx + 33;
            messagewrite[4] = axisy + 33;
            emit writesignal(firstplayer, connectip, messagewrite);
            // playertimes ++
            playertimes ++;
            // reset time
            time = 20;
            // change player
            checkboard->setPlayer(1);
        }
        else
        {
            playerwarnsecond();
            firstclick = true;
            secondclick = false;
            mylabels[firstclickx][firstclicky]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
            std::vector<MapNode*> proposals = generateproposals(firstclickx, firstclicky, checkboard->getNode(firstclickx, firstclicky));
            for(int i = 0; i < proposals.size(); i++)
            {
                int proposalx = proposals[i]->getx();
                int proposaly = proposals[i]->gety();
                mylabels[proposalx][proposaly]->setStyleSheet("QLabel{border:0px solid rgb(0, 120, 120);}");
            }
            // emit network signal

            messagewrite[0] = '4';
            messagewrite[1] = firstclickx + 33;
            messagewrite[2] = firstclicky + 33;
            emit writesignal(firstplayer, connectip, messagewrite);

        }
    }else if(checkboard->getNode(axisx, axisy) >= 0 && secondclick && (checkboard->getNode(axisx, axisy) % 2 != yourplayer))
    {
        int selected = checkboard->getNode(axisx, axisy);
        secondnode = new ChessNode(selected);
        if(lawful(firstclickx, firstclicky, firstnode, axisx, axisy, secondnode))
        {
            std::vector<MapNode*> proposals = generateproposals(firstclickx, firstclicky, checkboard->getNode(firstclickx, firstclicky));
            for(int i = 0; i < proposals.size(); i++)
            {
                int proposalx = proposals[i]->getx();
                int proposaly = proposals[i]->gety();
                mylabels[proposalx][proposaly]->setStyleSheet("QLabel{border:0px solid rgb(0, 120, 120);}");
            }
            updatecollision(firstclickx, firstclicky, firstnode, axisx, axisy, secondnode);
            firstclick = true;
            secondclick = false;
            mylabels[firstclickx][firstclicky]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
            // Send the network signal
            initmessagewrite();
            messagewrite[0] = '6';
            messagewrite[1] = firstclickx + 33;
            messagewrite[2] = firstclicky + 33;
            messagewrite[3] = axisx + 33;
            messagewrite[4] = axisy + 33;
            emit writesignal(firstplayer, connectip, messagewrite);
            playertimes ++;
            time = 20;
            checkboard->setPlayer(1);
        }
        else
        {
            playerwarnsecond();
            firstclick = true;
            secondclick = false;
            mylabels[firstclickx][firstclicky]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
            std::vector<MapNode*> proposals = generateproposals(firstclickx, firstclicky, checkboard->getNode(firstclickx, firstclicky));
            for(int i = 0; i < proposals.size(); i++)
            {
                int proposalx = proposals[i]->getx();
                int proposaly = proposals[i]->gety();
                mylabels[proposalx][proposaly]->setStyleSheet("QLabel{border:0px solid rgb(0, 120, 120);}");
            }

            messagewrite[0] = '4';
            messagewrite[1] = firstclickx + 33;
            messagewrite[2] = firstclicky + 33;
            emit writesignal(firstplayer, connectip, messagewrite);

        }
    }else if(checkboard->getNode(axisx, axisy) >= 0 && secondclick && (checkboard->getNode(axisx, axisy) % 2 == yourplayer))
    {
        playerwarnsecond();
        firstclick = true;
        secondclick = false;
        mylabels[firstclickx][firstclicky]->setStyleSheet("QLabel{border:0px solid rgb(0, 255, 0);}");
        std::vector<MapNode*> proposals = generateproposals(firstclickx, firstclicky, checkboard->getNode(firstclickx, firstclicky));
        for(int i = 0; i < proposals.size(); i++)
        {
            int proposalx = proposals[i]->getx();
            int proposaly = proposals[i]->gety();
            mylabels[proposalx][proposaly]->setStyleSheet("QLabel{border:0px solid rgb(0, 120, 120);}");
        }

        messagewrite[0] = '4';
        messagewrite[1] = firstclickx + 33;
        messagewrite[2] = firstclicky + 33;
        emit writesignal(firstplayer, connectip, messagewrite);

    }
    }
    else
        playerwarn();
    }
    else
    {
        if(checkboard->getNode(axisx, axisy) == -3 && firstclick && checkboard->getPlayer() == 0)
        {
            int nodetoget = checkboard->getinit(axisx, axisy);
            display(axisx, axisy, nodetoget);
            checkboard->setNode(axisx, axisy, nodetoget);
            //Send the network signal to update checkboard
            initmessagewrite();
            if(lastcolors.empty() || ((checkboard->getinit(axisx, axisy) % 2) != lastcolors.top()))
            {
                messagewrite[0] = '1';
                messagewrite[1] = '0';
                messagewrite[2] = axisx + 33;
                messagewrite[3] = axisy + 33;
                emit writesignal(firstplayer, connectip, messagewrite);
                //playertimes ++
                playertimes ++;
                //change current player
                checkboard->setPlayer(1);
                //save the last color
                int color = checkboard->getinit(axisx, axisy) % 2;
                lastcolors.push(color);
                time = 20;
            }else
            {
                messagewrite[0] = '1';
                messagewrite[1] = '1';
                messagewrite[2] = lastcolors.top() + '0';
                messagewrite[3] = axisx + 33;
                messagewrite[4] = axisy + 33;
                emit writesignal(firstplayer, connectip, messagewrite);
                playertimes ++;
                checkboard->setPlayer(1);
                yourplayer = lastcolors.top();
                chosenplayer = true;
                time = 20;
            }
        }else
            playerwarn();
    }
}

void MainWindow::display(int axisx, int axisy, int nodetoget)
{
    ChessNode* chessnode = new ChessNode(nodetoget);
    qDebug()<<chessnode->getname();
    QPixmap image = chessnode->geticon();
    mylabels[axisx][axisy]->setPixmap(image);
    mylabels[axisx][axisy]->show();
}

void MainWindow::updatecollision(int x1, int y1, ChessNode *node1, int x2, int y2, ChessNode *node2)
{
    if(bigger(node1, node2))
    {
        display(x1, y1, checkboard->original(x1, y1));
        display(x2, y2, node1->getid());
        checkboard->setNode(x1, y1, checkboard->original(x1, y1));
        checkboard->setNode(x2, y2, node1->getid());
    }else if(equal(node1, node2))
    {
        display(x1, y1, checkboard->original(x1, y1));
        display(x2, y2, checkboard->original(x2, y2));
        checkboard->setNode(x1, y1, checkboard->original(x1, y1));
        checkboard->setNode(x2, y2, checkboard->original(x2, y2));
    }else if(bigger(node2, node1))
    {
        display(x1, y1, checkboard->original(x1, y1));
        checkboard->setNode(x1, y1, checkboard->original(x1, y1));
    }
}

bool MainWindow::bigger(ChessNode* node1, ChessNode* node2)
{
    int state1 = node1->getid();
    int state2 = node2->getid();
    if(state2 == 0 || state2 == 1)
        return true;
    if((state1 >= 12 && state1 <= 17) && (state2 >= 2 && state2 <= 7))
        return true;
    if((state1 >= 18 && state1 <= 23) && (state2 >= 12 && state2 <= 17))
        return true;
    if((state1 >= 24 && state1 <= 29) && (state2 >= 12 && state2 <= 23))
        return true;
    if((state1 >= 30 && state1 <= 33) && (state2 >= 12 && state2 <= 29))
        return true;
    if((state1 >= 34 && state1 <= 37) && (state2 >= 12 && state2 <= 33))
        return true;
    if((state1 >= 38 && state1 <= 41) && (state2 >= 12 && state2 <= 37))
        return true;
    if((state1 >= 42 && state1 <= 45) && (state2 >= 12 && state2 <= 41))
        return true;
    if((state1 >= 46 && state1 <= 47) && (state2 >= 12 && state2 <= 45))
        return true;
    if((state1 >= 48 && state1 <= 49) && (state2 >= 12 && state2 <= 47))
        return true;
    return false;
}

bool MainWindow::equal(ChessNode* node1, ChessNode* node2)
{
    int state1 = node1->getid();
    int state2 = node2->getid();
    if((state1 >= 8 && state1 <= 11) || (state2 >= 8 && state2 <= 11))
        return true;
    if((state1 >= 12 && state1 <= 17) && (state2 >= 12 && state2 <= 17))
        return true;
    if((state1 >= 18 && state1 <= 23) && (state2 >= 18 && state2 <= 23))
        return true;
    if((state1 >= 24 && state1 <= 29) && (state2 >= 24 && state2 <= 29))
        return true;
    if((state1 >= 30 && state1 <= 33) && (state2 >= 30 && state2 <= 33))
        return true;
    if((state1 >= 34 && state1 <= 37) && (state2 >= 34 && state2 <= 37))
        return true;
    if((state1 >= 38 && state1 <= 41) && (state2 >= 38 && state2 <= 41))
        return true;
    if((state1 >= 42 && state1 <= 45) && (state2 >= 42 && state2 <= 45))
        return true;
    if((state1 >= 46 && state1 <= 47) && (state2 >= 46 && state2 <= 47))
        return true;
    if((state1 >= 48 && state1 <= 49) && (state2 >= 48 && state2 <= 49))
        return true;
    return false;
}

bool MainWindow::lawful(int x1, int y1, ChessNode* node1, int x2, int y2, ChessNode* node2)
{
    int state2 = node2->getid();
    int state1 = node1->getid();
    if(state1 <= 7)
        return false;
    if(!reachable(x1, y1, state1, x2, y2))
    {
        //qDebug() << reachable(x1, y1, state1, x2, y2);
        return false;
    }
    if(state2 < 0)
        return true;
    if(checkboard->original(x2, y2) == -1)
        return false;
    if(special(state1, state2))// 第一个不是工兵或炸弹，第二个是地雷或者厂上仍有地雷存在而第二个是军旗
        return false;
    ChessNode* nodeone = new ChessNode(state1);
    ChessNode* nodetwo = new ChessNode(state2);
    if(bigger(nodetwo, nodeone))
        return false;
    return true;
}

bool MainWindow::special(int node1, int node2)
{
    if((node2 >=2 && node2 <=7) && (!(node1 >= 8 && node1 <=17)))
        return true;
    if(node2 == 0 || node2 == 1)
    {
        int bannerside;
        if(yourplayer == 0)
            bannerside = 1;
        else
            bannerside = 0;
        if(checkboard->remainlandmine(bannerside))
            return true;
        else
        {
            emit winsignal();
            return false;
        }
    }
    return false;
}

bool MainWindow::reachable(int x1, int y1, int node, int x2, int y2)
{
    //  generate the proposals for x1, y1 and node
    //  if mapnode(x2, y2) inside proposals, return true
    std::vector<MapNode*> proposals = generateproposals(x1, y1, node);
    for(int i = 0; i < proposals.size(); i++)
        if(proposals[i]->getx() == x2 && proposals[i]->gety() == y2)
            return true;
    return false;
}

std::vector<MapNode*> MainWindow::generateproposals(int axisx, int axisy, int node)
{
    std::vector<MapNode*> result;
    if(axisx == 0 && axisy == 0)
    {
        MapNode* res1 = new MapNode(1, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(0, 1);
        result.push_back(res2);
    }else if(axisx == 1 && axisy == 0)
    {
        MapNode* res1 = new MapNode(0, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 0);
        result.push_back(res2);
        MapNode* res3 = new MapNode(1, 1);
        result.push_back(res3);
    }else if(axisx == 2 && axisy == 0)
    {
        MapNode* res1 = new MapNode(1, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 0);
        result.push_back(res2);
        MapNode* res3 = new MapNode(2, 1);
        result.push_back(res3);
    }else if(axisx == 3 && axisy == 0)
    {
        MapNode* res1 = new MapNode(2, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(4, 0);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 1);
        result.push_back(res3);
    }else if(axisx == 4 && axisy == 0)
    {
        MapNode* res1 = new MapNode(3, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(4, 1);
        result.push_back(res2);
    }else if(axisx == 1 && axisy == 2)
    {
        MapNode* res1 = new MapNode(0, 1);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 1);
        result.push_back(res2);
        MapNode* res3 = new MapNode(2, 1);
        result.push_back(res3);
        MapNode* res4 = new MapNode(0, 2);
        result.push_back(res4);
        MapNode* res5 = new MapNode(2, 2);
        result.push_back(res5);
        MapNode* res6 = new MapNode(0, 3);
        result.push_back(res6);
        MapNode* res7 = new MapNode(1, 3);
        result.push_back(res7);
        MapNode* res8 = new MapNode(2, 3);
        result.push_back(res8);
    }else if(axisx == 3 && axisy == 2)
    {
        MapNode* res1 = new MapNode(2, 1);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 1);
        result.push_back(res2);
        MapNode* res3 = new MapNode(4, 1);
        result.push_back(res3);
        MapNode* res4 = new MapNode(2, 2);
        result.push_back(res4);
        MapNode* res5 = new MapNode(4, 2);
        result.push_back(res5);
        MapNode* res6 = new MapNode(2, 3);
        result.push_back(res6);
        MapNode* res7 = new MapNode(3, 3);
        result.push_back(res7);
        MapNode* res8 = new MapNode(4, 3);
        result.push_back(res8);
    }else if(axisx == 2 && axisy == 3)
    {
        MapNode* res1 = new MapNode(1, 2);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 2);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 2);
        result.push_back(res3);
        MapNode* res4 = new MapNode(1, 3);
        result.push_back(res4);
        MapNode* res5 = new MapNode(3, 3);
        result.push_back(res5);
        MapNode* res6 = new MapNode(1, 4);
        result.push_back(res6);
        MapNode* res7 = new MapNode(2, 4);
        result.push_back(res7);
        MapNode* res8 = new MapNode(3, 4);
        result.push_back(res8);
    }else if(axisx == 1 && axisy == 4)
    {
        MapNode* res1 = new MapNode(0, 3);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 3);
        result.push_back(res2);
        MapNode* res3 = new MapNode(2, 3);
        result.push_back(res3);
        MapNode* res4 = new MapNode(0, 4);
        result.push_back(res4);
        MapNode* res5 = new MapNode(2, 4);
        result.push_back(res5);
        MapNode* res6 = new MapNode(0, 5);
        result.push_back(res6);
        MapNode* res7 = new MapNode(1, 5);
        result.push_back(res7);
        MapNode* res8 = new MapNode(2, 5);
        result.push_back(res8);
    }else if(axisx == 3 && axisy == 4)
    {
        MapNode* res1 = new MapNode(2, 3);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 3);
        result.push_back(res2);
        MapNode* res3 = new MapNode(4, 3);
        result.push_back(res3);
        MapNode* res4 = new MapNode(2, 4);
        result.push_back(res4);
        MapNode* res5 = new MapNode(4, 4);
        result.push_back(res5);
        MapNode* res6 = new MapNode(2, 5);
        result.push_back(res6);
        MapNode* res7 = new MapNode(3, 5);
        result.push_back(res7);
        MapNode* res8 = new MapNode(4, 5);
        result.push_back(res8);
    }else if(axisx == 1 && axisy == 7)
    {
        MapNode* res1 = new MapNode(0, 6);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 6);
        result.push_back(res2);
        MapNode* res3 = new MapNode(2, 6);
        result.push_back(res3);
        MapNode* res4 = new MapNode(0, 7);
        result.push_back(res4);
        MapNode* res5 = new MapNode(2, 7);
        result.push_back(res5);
        MapNode* res6 = new MapNode(0, 8);
        result.push_back(res6);
        MapNode* res7 = new MapNode(1, 8);
        result.push_back(res7);
        MapNode* res8 = new MapNode(2, 8);
        result.push_back(res8);
    }else if(axisx == 3 && axisy == 7)
    {
        MapNode* res1 = new MapNode(2, 6);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 6);
        result.push_back(res2);
        MapNode* res3 = new MapNode(4, 6);
        result.push_back(res3);
        MapNode* res4 = new MapNode(2, 7);
        result.push_back(res4);
        MapNode* res5 = new MapNode(4, 7);
        result.push_back(res5);
        MapNode* res6 = new MapNode(2, 8);
        result.push_back(res6);
        MapNode* res7 = new MapNode(3, 8);
        result.push_back(res7);
        MapNode* res8 = new MapNode(4, 8);
        result.push_back(res8);
    }else if(axisx == 2 && axisy == 8)
    {
        MapNode* res1 = new MapNode(1, 7);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 7);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 7);
        result.push_back(res3);
        MapNode* res4 = new MapNode(1, 8);
        result.push_back(res4);
        MapNode* res5 = new MapNode(3, 8);
        result.push_back(res5);
        MapNode* res6 = new MapNode(1, 9);
        result.push_back(res6);
        MapNode* res7 = new MapNode(2, 9);
        result.push_back(res7);
        MapNode* res8 = new MapNode(3, 9);
        result.push_back(res8);
    }else if(axisx == 1 && axisy == 9)
    {
        MapNode* res1 = new MapNode(0, 8);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 8);
        result.push_back(res2);
        MapNode* res3 = new MapNode(2, 8);
        result.push_back(res3);
        MapNode* res4 = new MapNode(0, 9);
        result.push_back(res4);
        MapNode* res5 = new MapNode(2, 9);
        result.push_back(res5);
        MapNode* res6 = new MapNode(0, 10);
        result.push_back(res6);
        MapNode* res7 = new MapNode(1, 10);
        result.push_back(res7);
        MapNode* res8 = new MapNode(2, 10);
        result.push_back(res8);
    }else if(axisx == 3 && axisy == 9)
    {
        MapNode* res1 = new MapNode(2, 8);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 8);
        result.push_back(res2);
        MapNode* res3 = new MapNode(4, 8);
        result.push_back(res3);
        MapNode* res4 = new MapNode(2, 9);
        result.push_back(res4);
        MapNode* res5 = new MapNode(4, 9);
        result.push_back(res5);
        MapNode* res6 = new MapNode(2, 10);
        result.push_back(res6);
        MapNode* res7 = new MapNode(3, 10);
        result.push_back(res7);
        MapNode* res8 = new MapNode(4, 10);
        result.push_back(res8);
    }else if(axisx == 2 && axisy == 2)
    {
        MapNode* res1 = new MapNode(2, 1);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 2);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 2);
        result.push_back(res3);
        MapNode* res4 = new MapNode(2, 3);
        result.push_back(res4);
    }else if(axisx == 1 && axisy == 3)
    {
        MapNode* res1 = new MapNode(1, 2);
        result.push_back(res1);
        MapNode* res2 = new MapNode(0, 3);
        result.push_back(res2);
        MapNode* res3 = new MapNode(2, 3);
        result.push_back(res3);
        MapNode* res4 = new MapNode(1, 4);
        result.push_back(res4);
    }else if(axisx == 3 && axisy == 3)
    {
        MapNode* res1 = new MapNode(3, 2);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 3);
        result.push_back(res2);
        MapNode* res3 = new MapNode(4, 3);
        result.push_back(res3);
        MapNode* res4 = new MapNode(3, 4);
        result.push_back(res4);
    }else if(axisx == 2 && axisy == 4)
    {
        MapNode* res1 = new MapNode(2, 3);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 4);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 4);
        result.push_back(res3);
        MapNode* res4 = new MapNode(2, 5);
        result.push_back(res4);
    }else if(axisx == 2 && axisy == 7)
    {
        MapNode* res1 = new MapNode(2, 6);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 7);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 7);
        result.push_back(res3);
        MapNode* res4 = new MapNode(2, 8);
        result.push_back(res4);
    }else if(axisx == 1 && axisy == 8)
    {
        MapNode* res1 = new MapNode(1, 7);
        result.push_back(res1);
        MapNode* res2 = new MapNode(0, 8);
        result.push_back(res2);
        MapNode* res3 = new MapNode(2, 8);
        result.push_back(res3);
        MapNode* res4 = new MapNode(1, 9);
        result.push_back(res4);
    }else if(axisx == 3 && axisy == 8)
    {
        MapNode* res1 = new MapNode(3, 7);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 8);
        result.push_back(res2);
        MapNode* res3 = new MapNode(4, 8);
        result.push_back(res3);
        MapNode* res4 = new MapNode(3, 9);
        result.push_back(res4);
    }else if(axisx == 2 && axisy == 9)
    {
        MapNode* res1 = new MapNode(2, 8);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 9);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 9);
        result.push_back(res3);
        MapNode* res4 = new MapNode(2, 10);
        result.push_back(res4);
    }else if(axisx == 0 && axisy == 11)
    {
        MapNode* res1 = new MapNode(0, 10);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 11);
        result.push_back(res2);
    }else if(axisx == 1 && axisy == 11)
    {
        MapNode* res1 = new MapNode(0, 11);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 11);
        result.push_back(res2);
        MapNode* res3 = new MapNode(1, 10);
        result.push_back(res3);
    }else if(axisx == 2 && axisy == 11)
    {
        MapNode* res1 = new MapNode(1, 11);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 11);
        result.push_back(res2);
        MapNode* res3 = new MapNode(2, 10);
        result.push_back(res3);
    }else if(axisx == 3 && axisy == 11)
    {
        MapNode* res1 = new MapNode(2, 11);
        result.push_back(res1);
        MapNode* res2 = new MapNode(4, 11);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 10);
        result.push_back(res3);
    }else if(axisx == 4 && axisy == 11)
    {
        MapNode* res1 = new MapNode(3, 11);
        result.push_back(res1);
        MapNode* res2 = new MapNode(4, 10);
        result.push_back(res2);
    }else if(axisx == 0 && axisy == 1)
    {
        MapNode* res1 = new MapNode(0, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 2);
        result.push_back(res2);
    }else if(axisx == 1 &&  axisy == 1)
    {
        MapNode* res1 = new MapNode(1, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 2);
        result.push_back(res2);
    }else if(axisx == 2 && axisy == 1)
    {
        MapNode* res1 = new MapNode(2, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 2);
        result.push_back(res2);
        MapNode* res3 = new MapNode(1, 2);
        result.push_back(res3);
        MapNode* res4 = new MapNode(3, 2);
        result.push_back(res4);
    }else if(axisx == 3 &&  axisy == 1)
    {
        MapNode* res1 = new MapNode(3, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 2);
        result.push_back(res2);
    }else if(axisx == 4 && axisy == 1)
    {
        MapNode* res1 = new MapNode(4, 0);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 2);
        result.push_back(res2);
    }else if(axisx == 0 && axisy == 2)
    {
        MapNode* res1 = new MapNode(1, 2);
        result.push_back(res1);
    }else if(axisx == 4 && axisy == 2)
    {
        MapNode* res1 = new MapNode(3, 2);
        result.push_back(res1);
    }else if(axisx == 0 && axisy == 3)
    {
        MapNode* res1 = new MapNode(1, 2);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 3);
        result.push_back(res2);
        MapNode* res3 = new MapNode(1, 4);
        result.push_back(res3);
    }else if(axisx == 4 && axisy == 3)
    {
        MapNode* res1 = new MapNode(3, 2);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 3);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 4);
        result.push_back(res3);
    }else if(axisx == 0 && axisy == 4)
    {
        MapNode* res1 = new MapNode(1, 4);
        result.push_back(res1);
    }else if(axisx == 4 && axisy == 4)
    {
        MapNode* res1 = new MapNode(3, 4);
        result.push_back(res1);
    }else if(axisx == 0 && axisy == 5)
    {
        MapNode* res1 = new MapNode(1, 4);
        result.push_back(res1);
    }else if(axisx == 4 && axisy == 5)
    {
        MapNode* res1 = new MapNode(3, 4);
        result.push_back(res1);
    }else if(axisx == 1 && axisy == 5)
    {
        MapNode* res1 = new MapNode(1, 4);
        result.push_back(res1);
    }else if(axisx == 3 && axisy == 5)
    {
        MapNode* res1 = new MapNode(3, 4);
        result.push_back(res1);
    }else if(axisx == 2 && axisy == 5)
    {
        MapNode* res1 = new MapNode(1, 4);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 4);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 4);
        result.push_back(res3);
    }else if(axisx == 0 && axisy == 6)
    {
        MapNode* res1 = new MapNode(1, 7);
        result.push_back(res1);
    }else if(axisx == 4 && axisy == 6)
    {
        MapNode* res1 = new MapNode(3, 7);
        result.push_back(res1);
    }else if(axisx == 1 && axisy == 6)
    {
        MapNode* res1 = new MapNode(1, 7);
        result.push_back(res1);
    }else if(axisx == 3 && axisy == 6)
    {
        MapNode* res1 = new MapNode(3, 7);
        result.push_back(res1);
    }else if(axisx == 2 && axisy == 6)
    {
        MapNode* res1 = new MapNode(1, 7);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 7);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 7);
        result.push_back(res3);
    }else if(axisx == 0 && axisy == 7)
    {
        MapNode* res1 = new MapNode(1, 7);
        result.push_back(res1);
    }else if(axisx == 4 && axisy == 7)
    {
        MapNode* res1 = new MapNode(3, 7);
        result.push_back(res1);
    }else if(axisx == 0 && axisy == 8)
    {
        MapNode* res1 = new MapNode(1, 7);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 8);
        result.push_back(res2);
        MapNode* res3 = new MapNode(1, 9);
        result.push_back(res3);
    }else if(axisx == 4 && axisy == 8)
    {
        MapNode* res1 = new MapNode(3, 7);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 8);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 9);
        result.push_back(res3);
    }else if(axisx == 0 && axisy == 9)
    {
        MapNode* res1 = new MapNode(1, 9);
        result.push_back(res1);
    }else if(axisx == 4 && axisy == 9)
    {
        MapNode* res1 = new MapNode(3, 9);
        result.push_back(res1);
    }else if(axisx == 0 && axisy == 10)
    {
        MapNode* res1 = new MapNode(1, 9);
        result.push_back(res1);
        MapNode* res2 = new MapNode(0, 11);
        result.push_back(res2);
    }else if(axisx == 4 && axisy == 10)
    {
        MapNode* res1 = new MapNode(3, 9);
        result.push_back(res1);
        MapNode* res2 = new MapNode(4, 11);
        result.push_back(res2);
    }else if(axisx == 1 && axisy == 10)
    {
        MapNode* res1 = new MapNode(1, 9);
        result.push_back(res1);
        MapNode* res2 = new MapNode(1, 11);
        result.push_back(res2);
    }else if(axisx == 3 && axisy == 10)
    {
        MapNode* res1 = new MapNode(3, 9);
        result.push_back(res1);
        MapNode* res2 = new MapNode(3, 11);
        result.push_back(res2);
    }else if(axisx == 2 && axisy == 10)
    {
        MapNode* res1 = new MapNode(1, 9);
        result.push_back(res1);
        MapNode* res2 = new MapNode(2, 9);
        result.push_back(res2);
        MapNode* res3 = new MapNode(3, 9);
        result.push_back(res3);
        MapNode* res4 = new MapNode(2, 11);
        result.push_back(res4);
    }
    // if you are reading codes, please ignore the following dumb codes for I wrote them when I was extremely dizzy
    // all you have to know is that they implement the reachable function on railway for non-sappers
    if(!(node >= 12 && node <= 17))
    {
        if(axisx == 0 && axisy == 1)
        {
            for(int i = 1; i <= 4; i++)
            {
                if(checkboard->getNode(i, 1) == -2)
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 2; j <= 10; j++)
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
        }else if(axisx == 4 && axisy == 1)
        {
            for(int i = 3; i >= 0; i--)
            {
                if(checkboard->getNode(i, 1) == -2)
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 2; j <= 10; j++)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 0 && axisy == 10)
        {
            for(int j = 9; j>= 1; j--)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int i = 1; i <= 4; i++)
            {
                if(checkboard->getNode(i, 10) == -2)
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 4 && axisy == 10)
        {
            for(int i = 3; i >= 0; i--)
            {
                if(checkboard->getNode(i, 10) == -2)
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 9; j >= 1; j--)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 0 && axisy == 5)
        {
            for(int j = 4; j >= 1; j--)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 6; j <= 10; j++)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }for(int i = 1; i <= 4; i++)
            {
                if(checkboard->getNode(i, 5) == -2)
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 4 && axisy == 5)
        {
            for(int i = 3; i >= 0; i--)
            {
                if(checkboard->getNode(i, 5) == -2)
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 4; j >= 1; j--)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }for(int j = 6; j <= 10; j++)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 0 && axisy == 6)
        {
            for(int j = 5; j >= 1; j--)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 7; j <= 10; j++)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int i = 1; i <= 4; i++)
            {
                if(checkboard->getNode(i, 6) == -2)
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 4 && axisy == 6)
        {
            for(int i = 3; i >= 0; i--)
            {
                if(checkboard->getNode(i, 6) == -2)
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 5; j >= 1; j--)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 7; j <= 10; j++)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 2 && axisy == 5)
        {
            MapNode* res10 = new MapNode(2, 6);
            result.push_back(res10);
            for(int i = 1; i >= 0; i--)
            {
                if(checkboard->getNode(i, 5) == -2)
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                    break;
                }
            }
            for(int i = 3; i <= 4; i++)
            {
                if(checkboard->getNode(i, 5) == -2)
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 2 && axisy == 6)
        {
            MapNode* res = new MapNode(2, 5);
            result.push_back(res);
            for(int i = 1; i >= 0; i--)
            {
                if(checkboard->getNode(i, 6) == -2)
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                    break;
                }
            }
            for(int i = 3; i <= 4; i++)
            {
                if(checkboard->getNode(i, 6) == -2)
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 1 && axisy == 1)
        {
            MapNode* res10 = new MapNode(0, 1);
            result.push_back(res10);
            for(int i = 2; i <= 4; i++)
            {
                if(checkboard->getNode(i, 1) == -2)
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 2 && axisy == 1)
        {
            for(int i = 1; i >= 0; i--)
            {
                if(checkboard->getNode(i, 1) == -2)
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                    break;
                }
            }
            for(int i = 3; i <= 4; i++)
            {
                if(checkboard->getNode(i, 1) == -2)
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 3 && axisy == 1)
        {
            MapNode* res10 = new MapNode(4, 1);
            result.push_back(res10);
            for(int i = 2; i >= 0; i--)
            {
                if(checkboard->getNode(i, 1) == -2)
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 1);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 1 && axisy == 5)
        {
            MapNode* res10 = new MapNode(0, 5);
            result.push_back(res10);
            for(int i = 2; i <= 4; i++)
            {
                if(checkboard->getNode(i, 5) == -2)
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 3 && axisy == 5)
        {
            MapNode* res11 = new MapNode(4, 5);
            result.push_back(res11);
            for(int i = 2; i >= 0; i--)
            {
                if(checkboard->getNode(i, 5) == -2)
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 5);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 1 && axisy == 6)
        {
            MapNode* res10 = new MapNode(0, 6);
            result.push_back(res10);
            for(int i = 2; i <= 4; i++)
            {
                if(checkboard->getNode(i, 6) == -2)
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 3 && axisy == 6)
        {
            MapNode* res11 = new MapNode(4, 6);
            result.push_back(res11);
            for(int i = 2; i >= 0; i--)
            {
                if(checkboard->getNode(i, 6) == -2)
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 6);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 1 && axisy == 10)
        {
            MapNode* res10 = new MapNode(0, 10);
            result.push_back(res10);
            for(int i = 2; i <= 4; i++)
            {
                if(checkboard->getNode(i, 10) == -2)
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 2 && axisy == 10)
        {
            for(int i = 1; i >= 0; i--)
            {
                if(checkboard->getNode(i, 10) == -2)
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                    break;
                }
            }
            for(int i = 3; i <= 4; i++)
            {
                if(checkboard->getNode(i, 10) == -2)
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 3 && axisy == 10)
        {
            MapNode* res11 = new MapNode(4, 10);
            result.push_back(res11);
            for(int i = 2; i >= 0; i--)
            {
                if(checkboard->getNode(i, 10) == -2)
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(i, 10);
                    result.push_back(res);
                    break;
                }
            }
        }
        // //
        else if(axisx ==  0 && axisy == 2)
        {
            MapNode* res10 = new MapNode(0, 1);
            result.push_back(res10);
            for(int j = 3; j <= 10; j++)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 0 && axisy == 3)
        {
            for(int j = 2; j >= 1; j--)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 4; j <= 10; j++)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 0 && axisy == 4)
        {
            for(int j = 3; j >= 1; j--)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 5; j <= 10; j++)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
        }
        else if(axisx ==  4 && axisy == 2)
        {
            MapNode* res10 = new MapNode(4, 1);
            result.push_back(res10);
            for(int j = 3; j <= 10; j++)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 4 && axisy == 3)
        {

            for(int j = 2; j >= 0; j--)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 4; j <= 10; j++)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 4 && axisy == 4)
        {
            for(int j = 3; j >= 1; j--)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = 5; j <= 10; j++)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 0 && axisy >= 7 && axisy <= 9)
        {
            for(int j = axisy - 1; j >= 1; j--)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = axisy + 1; j <= 10; j++)
            {
                if(checkboard->getNode(0, j) == -2)
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(0, j);
                    result.push_back(res);
                    break;
                }
            }
        }else if(axisx == 4 && axisy >= 7 && axisy <= 9)
        {
            for(int j = axisy - 1; j >= 1; j--)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
            for(int j = axisy + 1; j <= 10; j++)
            {
                if(checkboard->getNode(4, j) == -2)
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                }else
                {
                    MapNode* res = new MapNode(4, j);
                    result.push_back(res);
                    break;
                }
            }
        }
    }else if((axisx == 0 && axisy >= 1 && axisy <= 10) || (axisx == 4 && axisy >= 1 && axisy <= 10)
             || (((axisy == 1) || (axisy == 5) || (axisy == 6) || (axisy == 10)) && axisx >= 1 && axisx <= 3))
    // using dfs to solve the sapper railway problem
    {
            for(int i = 0; i < 5; i ++)
                for(int j = 0; j < 12; j++)
                      searched[i][j] = false;
            dfs(axisx, axisy, result);
    }
    return result;
}

void MainWindow::dfs(int axisx, int axisy, std::vector<MapNode*>& result)
{
    searched[axisx][axisy] = true;
    MapNode* current = new MapNode(axisx, axisy);
    for(int i = 0; i < current->neighborx.size(); i++)
    {
        int xtosearch = current->neighborx[i];
        int ytosearch = current->neighbory[i];
        qDebug()<<xtosearch<<ytosearch;
        if(!searched[xtosearch][ytosearch])
        {
            if(checkboard->getNode(xtosearch, ytosearch) == -2)
            {
                MapNode* endchessnode = new MapNode(xtosearch, ytosearch);
                result.push_back(endchessnode);
                dfs(xtosearch, ytosearch, result);
            }
            else
            {
                MapNode* endchessnode = new MapNode(xtosearch, ytosearch);
                result.push_back(endchessnode);
                searched[xtosearch][ytosearch] = true;
            }
        }
    }
}

void MainWindow::playerwarn()
{
    QWidget* widg = new QWidget();
    widg->setFixedSize(400, 400);
    QLabel* labforwidg = new QLabel(widg);
    labforwidg->move(100, 200);
    labforwidg->setText("Please select the lawful place!");
    widg->show();
}

void MainWindow::playerwarnsecond()
{
    QWidget* widg = new QWidget();
    widg->setFixedSize(400, 400);
    QLabel* labforwidg = new QLabel(widg);
    labforwidg->move(80, 200);
    labforwidg->setText("Unlawful selction, please restart the operation!");
    widg->show();
}

bool MainWindow::alldiscovered()
{
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
            if(checkboard->getNode(i, j) == -3)
                return false;
    return true;
}

bool MainWindow::judgedefeat()
{
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 12; j++)
        {
            int state = checkboard->getNode(i, j);
            if(state >= 8 && (state % 2 == yourplayer))
            {
                std::vector<MapNode*> proposals = generateproposals(i, j, state);
                for(int k = 0; k < proposals.size(); k++)
                {
                    int desx = proposals[k]->getx();
                    int desy = proposals[k]->gety();
                    int desstate = checkboard->getNode(desx, desy);
                    ChessNode* chessnode1 = new ChessNode(state);
                    ChessNode* chessnode2 = new ChessNode(desstate);
                    if(lawful(i, j, chessnode1, desx, desy, chessnode2))
                        return false;
                }
            }
        }
    return true;
}

void MainWindow::settimerlabel()
{
    QString timercontent = "remaining time: ";
    timercontent += QString("%1").arg(time);
    timercontent += '\t';
    timercontent += "your time-exceed times: ";
    timercontent += QString("%1").arg(outtimes[0]);
    timercontent += "\nyour side: ";
    timercontent += yourside;
    timercontent += '\t';
    timercontent += '\t';
    timercontent += "current player: ";
    timercontent += currentplayer;

    timerlabel->setText(timercontent);
}

void MainWindow::initmessagewrite()
{
    for(int i = 0; i < 64; i++)
        messagewrite[i] = '0';
}

MainWindow::~MainWindow()
{
    delete ui;
    delete checkboard;
    mythread->quit();
    mythread->wait();
}

