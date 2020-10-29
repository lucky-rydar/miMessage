#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    this->serverConnection = new QTcpSocket(this);
    serverConnection->connectToHost(QHostAddress("127.0.0.1"), 333);

    connect(serverConnection, &QTcpSocket::readyRead, this, &Client::onServerMessasge);
}

void Client::registerUser(QString username, QString password)
{
    QJsonObject toSend;
    toSend["message-type"] = "registration";
    toSend["username"] = username;
    toSend["password"] = password;

    serverConnection->write(QJsonDocument(toSend).toJson());
}

void Client::onServerMessasge()
{

}
