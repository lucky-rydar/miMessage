#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    this->serverConnection = new QUdpSocket(this);
    serverConnection->bind(QHostAddress::Any, 444); // here may need to use address from private

    this->serverAddress = QHostAddress("127.0.0.1");
    this->serverPort = 333;
    connect(serverConnection, &QTcpSocket::readyRead, this, &Client::onServerMessasge);
}

void Client::registerUser(QString username, QString password)
{
    QJsonObject toSend;
    toSend["message-type"] = "registration";
    toSend["username"] = username;
    toSend["password"] = password;

    serverConnection->writeDatagram(QJsonDocument(toSend).toJson(), serverAddress, serverPort);
}

void Client::loginUser(QString username, QString password)
{
    QJsonObject toSend;
    toSend["message-type"] = "logining";
    toSend["username"] = username;
    toSend["password"] = password;

    serverConnection->writeDatagram(QJsonDocument(toSend).toJson(), serverAddress, serverPort);
}

void Client::addNewChatGroup(QString chatOrGroupName, QString chatOrGroup)
{
    QJsonObject toSend;
    toSend["username"] = this->username;
    toSend["password"] = this->password;
    toSend["message-type"] = "adding-chat-group";
    toSend["chat-or-group"] = chatOrGroup;
    toSend["chat-or-group-name"] = chatOrGroupName;
    qDebug() << toSend;

    serverConnection->writeDatagram(QJsonDocument(toSend).toJson(), serverAddress, serverPort);
}

void Client::sendMessageTo(QString messageText, QString chatName, QString chatOrGroup)
{
    QJsonObject toSend;
    toSend["username"] = this->username;
    toSend["password"] = this->password;
    toSend["message-type"] = "sending-message";
    toSend["message-text"] = messageText;
    toSend["chat-or-group"] = chatOrGroup;
    toSend["chat-or-group-name"] = chatName;
    toSend["date-time-string"] = QDateTime::currentDateTime().toString();
    qDebug() << toSend;

    serverConnection->writeDatagram(QJsonDocument(toSend).toJson(), serverAddress, serverPort);
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

        //qDebug() << buff;

        QJsonDocument doc = QJsonDocument::fromJson(buff);
        QJsonObject obj = doc.object();

        if(obj["username"].toString() == this->username)
        {
            qDebug() << "Received message" << obj;
            if(obj["message-type"] == "reg-status")
                emit Registered(obj["registered"].toBool());
            else if(obj["message-type"] == "login-status")
                emit Logined(obj["logined"].toBool(), obj);
            else if(obj["message-type"] == "adding-chat-group-status")
                emit AddedNewChat(obj["is-added"].toBool(), obj["chat-or-group-name"].toString(), obj["chat-id"].toInt());
            else if(obj["message-type"] == "new-message")
                emit newMessage(obj["chat-name"].toString(), obj["message-text"].toString());
        }

    }


}
