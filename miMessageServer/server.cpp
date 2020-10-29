#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    this->tcpServer = new QTcpServer(parent);
    this->dbController = new DatabaseController(parent);
    this->clientsList = QVector<QTcpSocket*>();

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);

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

void Server::newConnection()
{
    QTcpSocket *sock = tcpServer->nextPendingConnection();
    clientsList.push_back(sock);
    connect(sock, &QTcpSocket::disconnected, [=](){ this->clientsList.removeOne(sock); });

    qInfo() << "connected";
    // connect readyRead with reading
}
