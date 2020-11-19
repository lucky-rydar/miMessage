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
        toSend["username"] = clientMessage["username"].toString();

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
        }
        else if(clientMessage["message-type"] == "logining")
        {
            toSend["message-type"] = "login-status";
            toSend["logined"] = dbController->isRegistered(clientMessage["username"].toString(), QCryptographicHash::hash(clientMessage["password"].toString().toUtf8(), QCryptographicHash::Md5).toHex());
            //TODO: add the list of all chats too
            auto chatsList = dbController->getChatsByUsername(clientMessage["username"].toString());


            QJsonArray chatsInfo;
            for(int i = 0; i < chatsList.size(); i++)
            {
                QJsonObject chatInfo;
                chatInfo["chat-name"] = chatsList[i].chatName;
                chatInfo["chat-id"] = chatsList[i].chatId;

                chatsInfo.append(chatInfo);
            }
            toSend["chats-info"] = chatsInfo;

            qInfo() << "logining";
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
        else if(clientMessage["message-type"] == "sending-message")
        {
            qInfo() << "Sending message";
            toSend["message-type"] = "sent-message-status";
            Message received(clientMessage["chat-or-group-name"].toString(), clientMessage["message-text"].toString(), clientMessage["sending-date-time"].toString());
            dbController->addMessage(received, clientMessage["from-user"].toString(), clientMessage["to-user"].toString(), clientMessage["chat-or-group"].toString(),
                    clientMessage["chat-or-group-name"].toString());

            toSend["chat-name"] = clientMessage["chat-or-group-name"].toString();
            toSend["message-text"] = clientMessage["message-text"].toString();
            toSend["message-id"] = received.messageId;
            toSend["from-user"] = clientMessage["from-user"].toString();
            toSend["date-time"] = clientMessage["sending-date-time"].toString();

            QJsonObject toSecondUser = toSend;
            toSecondUser["username"] = clientMessage["to-user"].toString();
            udpServer->writeDatagram(QJsonDocument(toSecondUser).toJson(), senderIP, 444);
        }
        else if(clientMessage["message-type"] == "messages-request")
        {
            toSend["message-type"] = "message-list-for";
            toSend["chat-or-group-name"] = clientMessage["chat-or-group-name"];

            auto messages = dbController->getMessagesFor(clientMessage["chat-or-group-name"].toString(), clientMessage["chat-or-group"].toString());
            //auto messages = dbController->getMessagesFor("admin-misha", "chat");
            QJsonArray messagesJson;
            for(int i = 0; i < messages.size(); i++)
            {
                QJsonObject message;
                message["message-text"] = messages[i]->massageText;
                message["from-user"] = messages[i]->from;
                message["date-time"] = messages[i]->dateTime;
                message["message-id"] = messages[i]->messageId;
                messagesJson.append(message);


                delete messages[i];
            }
            toSend["messages-list"] = messagesJson;
        }
        toSend["username"] = clientMessage["username"];

        qInfo() << toSend;
        udpServer->writeDatagram(QJsonDocument(toSend).toJson(), senderIP, 444);
    }
}
