#ifndef MYSERVER_H
#define MYSERVER_H

#include <QThread>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    void Listen(bool, bool);

signals:
    void finishreading(char*);

private:
    QTcpServer* server;
    QTcpSocket* listensocket;
};

#endif // MYSERVER_H
