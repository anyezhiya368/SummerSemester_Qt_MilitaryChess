#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QThread>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MySocket : public QObject
{
    Q_OBJECT
public:
    explicit MySocket(QObject *parent = nullptr);
    void Write(bool, QString, char*);

signals:
   void finishwriting();
private:
    QTcpSocket* writesocket;

};

#endif // MYSOCKET_H
