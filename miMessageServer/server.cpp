#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    this->tcpServer = new QTcpServer(parent);
    this->dbController = new DatabaseController(parent);

    //TODO: connect all signals of QTcpServer
}

void Server::run()
{
    if(!tcpServer->listen(QHostAddress::Any, 333))
    {
        qCritical("Cant run the server");
        return;
    }

    qInfo("Server started successfully...");

    //TODO: do something if everything is fine
}
