#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    this->udpServer = new QUdpSocket(this);
    this->dbController = new DatabaseController(parent);

    connect(udpServer, &QUdpSocket::readyRead, this, &Server::onNewClientMessage);
}

void Server::run()
{
    if(!udpServer->bind(QHostAddress::Any, 333))
    {
        qCritical("Cant run the server");
        return;
    }

    qInfo("Server started successfully...");

    //TODO: do something if everything is fine
}

void Server::onNewClientMessage()
{
    while(udpServer->hasPendingDatagrams())
    {
        QByteArray buff;
        buff.resize(udpServer->pendingDatagramSize());
        QHostAddress senderIP;
        quint16 senderPort;
        udpServer->readDatagram(buff.data(), buff.size(), &senderIP, &senderPort);

        QJsonObject clientMessage = QJsonDocument::fromJson(buff).object();
        QJsonObject toSend;

        qDebug() << "new Message";

        if(clientMessage["message-type"] == "registration")
        {
            toSend["message-type"] = "reg-status";
            if(dbController->userExist(clientMessage["username"].toString()))
                toSend["registered"] = false;
            else
            {
                dbController->addUser(clientMessage["username"].toString(), QCryptographicHash::hash(clientMessage["password"].toString().toUtf8(), QCryptographicHash::Md5).toHex());
                toSend["registered"] = true;
            }
            qDebug() << toSend;
        }
        else if(clientMessage["message-type"] == "logining")
        {
            toSend["message-type"] = "login-status";
            toSend["logined"] = dbController->isRegistered(clientMessage["username"].toString(), QCryptographicHash::hash(clientMessage["password"].toString().toUtf8(), QCryptographicHash::Md5).toHex());
            //TODO: add the list of all chats too

            qDebug() << toSend;
        }

        udpServer->writeDatagram(QJsonDocument(toSend).toJson(), senderIP, 444);

    }




}
