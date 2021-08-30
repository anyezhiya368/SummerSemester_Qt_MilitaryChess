#include "mysocket.h"

MySocket::MySocket(QObject *parent) : QObject(parent)
{

}

void MySocket::Write(bool firstplayer, QString ip, char* message)
{
    writesocket = new QTcpSocket();
    if(firstplayer)
        writesocket->connectToHost(ip, 8888);
    else
        writesocket->connectToHost(ip, 6666);
    connect(writesocket, &QTcpSocket::connected,
            [=]()
            {
                writesocket->write(message);
            }
            );
    emit finishwriting();
}
