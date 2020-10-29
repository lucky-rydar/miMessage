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
    if(!clientsList.contains(sock))
        clientsList.push_back(sock);

    connect(sock, &QTcpSocket::disconnected, [=](){ this->clientsList.removeOne(sock); });

    connect(sock, &QTcpSocket::readyRead, [=](){
        onNewClientMessage(clientsList.indexOf(sock));
    });

    qInfo() << "connected";
    // connect readyRead with reading
}

void Server::onNewClientMessage(int sockIndex)
{
    QTcpSocket* userSock = clientsList.at(sockIndex);
    QJsonObject clientMessage = QJsonDocument::fromJson(userSock->readAll()).object();
    QJsonObject toSend;

    qDebug() << "new Message";

    if(clientMessage["message-type"] == "registration")
    {
        toSend["message-type"] = "reg-status";
        if(dbController->userExist(clientMessage["username"].toString()))
            toSend["registered"] = false;
        else
        {
            dbController->addUser(clientMessage["username"].toString(), QCryptographicHash::hash(clientMessage["password"].toString().toUtf8(), QCryptographicHash::Md5));
            toSend["registered"] = true;
        }
        qDebug() << toSend;
        userSock->write(QJsonDocument(toSend).toJson());
        userSock->flush();
    }


}
