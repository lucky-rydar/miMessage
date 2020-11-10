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
        qDebug() << clientMessage;

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
            qInfo() << "registration";
            qInfo() << toSend;
        }
        else if(clientMessage["message-type"] == "logining")
        {
            toSend["message-type"] = "login-status";
            toSend["logined"] = dbController->isRegistered(clientMessage["username"].toString(), QCryptographicHash::hash(clientMessage["password"].toString().toUtf8(), QCryptographicHash::Md5).toHex());
            //TODO: add the list of all chats too
            qInfo() << "logining";
            qInfo() << toSend;
        }
        else if(clientMessage["message-type"] == "adding-chat-group")
        {
            qInfo() << "Adding chat";
            toSend["message-type"] = "adding-chat-group-status";
            if(clientMessage["chat-or-group"] == "chat")
            {
                if(!dbController->chatExist(clientMessage["username"].toString(), clientMessage["chat-or-group-name"].toString()))
                {
                    QString chatName = dbController->usernamesToChatName(clientMessage["username"].toString(), clientMessage["chat-or-group-name"].toString());

                    toSend["is-added"] = dbController->addChat(clientMessage["username"].toString(), clientMessage["chat-or-group-name"].toString());
                    toSend["chat-or-group"] = "chat";
                    toSend["chat-or-group-name"] = chatName;
                    toSend["chat-id"] = dbController->chatIdByName(chatName);
                }
            }
            else if(clientMessage["chat-or-group"] == "group")
            {
                toSend["message-type"] = "adding-chat-group-status";
                //TODO: later add groups but it should looks such as a chat
            }
        }
        udpServer->writeDatagram(QJsonDocument(toSend).toJson(), senderIP, 444);
    }
}
