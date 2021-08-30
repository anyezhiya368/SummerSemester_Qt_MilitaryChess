#include "myserver.h"

MyServer::MyServer(QObject *parent) : QObject(parent)
{

}


void MyServer::Listen(bool closeserver, bool firstserver)
{
   server = new QTcpServer();
   if(closeserver)
   {
       server->close();
   }
   else{
   if(firstserver)
   {
       bool flag = server->listen(QHostAddress::Any, 6666);
       if(flag)
       {
           connect(server, &QTcpServer::newConnection, this,
                   [=]()
                    {
                        listensocket = server->nextPendingConnection();
                        connect(listensocket, &QTcpSocket::readyRead,
                                [=]()
                        {
                            char message[64];
                            listensocket->read(message, sizeof message);
                            emit finishreading(message);
                        }
                                );

           }
                   );
       }
   }else
   {
       bool flag = server->listen(QHostAddress::Any, 8888);
       if(flag)
       {
           connect(server, &QTcpServer::newConnection, this,
                   [=]()
                    {
                        listensocket = server->nextPendingConnection();
                        connect(listensocket, &QTcpSocket::readyRead,
                                [=]()
                        {
                            char message[64];
                            listensocket->read(message, sizeof message);
                            emit finishreading(message);
                        }
                                );

           }
                   );
       }
   }
}
}

