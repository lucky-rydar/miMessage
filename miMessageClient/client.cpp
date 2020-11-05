#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    this->serverConnection = new QUdpSocket(this);
    serverConnection->bind(QHostAddress::Any, 444);

    connect(serverConnection, &QTcpSocket::readyRead, this, &Client::onServerMessasge);
}

void Client::registerUser(QString username, QString password)
{
    QJsonObject toSend;
    toSend["message-type"] = "registration";
    toSend["username"] = username;
    toSend["password"] = password;

    serverConnection->writeDatagram(QJsonDocument(toSend).toJson(), QHostAddress::LocalHost, 333);
}

void Client::loginUser(QString username, QString password)
{
    QJsonObject toSend;
    toSend["message-type"] = "logining";
    toSend["username"] = username;
    toSend["password"] = password;

    serverConnection->writeDatagram(QJsonDocument(toSend).toJson(), QHostAddress::LocalHost, 333);
}

void Client::onServerMessasge()
{
    while(serverConnection->hasPendingDatagrams())
    {
        QByteArray buff;
        buff.resize(serverConnection->pendingDatagramSize());
        QHostAddress senderIP;
        quint16 senderPort;
        serverConnection->readDatagram(buff.data(), buff.size(), &senderIP, &senderPort);

        qDebug() << buff;

        QJsonDocument doc = QJsonDocument::fromJson(buff);
        QJsonObject obj = doc.object();

        qDebug() << obj;

        if(obj["message-type"] == "reg-status")
            emit Registered(obj["registered"].toBool());
        else if(obj["message-type"] == "login-status")
            emit Logined(obj["logined"].toBool());
    }


}
