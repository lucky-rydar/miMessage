#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    this->port = 333;
    this->sslServer = new QTcpServer(parent);
    this->dbController = new DatabaseController(parent);

    connect(sslServer, &QTcpServer::newConnection, this, &Server::onNewConnection);

    this->audioServer = new AudioServer(parent);
    connect(audioServer, &AudioServer::newCalling, this, &Server::onCallingToSomeone);
    connect(audioServer, &AudioServer::declined, [=](QString toUser){
        QJsonObject toSend;
        toSend["message-type"] = "calling-declined";
        if(this->socketByUsername[toUser] != nullptr)
        {
            this->socketByUsername[toUser]->write(QJsonDocument(toSend).toJson());
            this->socketByUsername[toUser]->flush();
        }
    });
    connect(audioServer, &AudioServer::accepted, [=](QString toUser){
        QJsonObject toSend;
        toSend["message-type"] = "calling-accepted";
        if(this->socketByUsername[toUser] != nullptr)
        {
            this->socketByUsername[toUser]->write(QJsonDocument(toSend).toJson());
            this->socketByUsername[toUser]->flush();
        }
    });

    connect(this->audioServer, &AudioServer::disconnected, this, &Server::onCallingEnds);
}

void Server::run()
{
    if(!sslServer->listen(QHostAddress::Any, port))
    {
        qCritical("Cant run base server");
        return;
    }
    qInfo("Base server started successfully...");

    this->audioServer->run();
}

void Server::onNewClientMessage()
{
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(sender());
    QByteArray buff;
    buff = sock->readAll();

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
    }
    else if(clientMessage["message-type"] == "logining")
    {
        toSend["message-type"] = "login-status";
        toSend["logined"] = dbController->isRegistered(clientMessage["username"].toString(), QCryptographicHash::hash(clientMessage["password"].toString().toUtf8(), QCryptographicHash::Md5).toHex());
            //TODO: add the list of all chats too
        auto chatsList = dbController->getChatsByUsername(clientMessage["username"].toString());
        this->socketByUsername[clientMessage["username"].toString()] = sock;
        this->usernameBySocket[sock] = clientMessage["username"].toString();

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
            if(socketByUsername[clientMessage["chat-or-group-name"].toString()] != nullptr)
            {
                socketByUsername[clientMessage["chat-or-group-name"].toString()]->write(QJsonDocument(toSend).toJson());
                socketByUsername[clientMessage["chat-or-group-name"].toString()]->flush();
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
        toSecondUser["message-type"] = "new-message";
        if(socketByUsername[clientMessage["to-user"].toString()] != nullptr)
        {
            this->socketByUsername[clientMessage["to-user"].toString()]->write(QJsonDocument(toSecondUser).toJson());
            this->socketByUsername[clientMessage["to-user"].toString()]->flush();
        }
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

    qInfo() << toSend;
    sock->write(QJsonDocument(toSend).toJson());
    sock->flush();
}

void Server::onNewConnection()
{
    qInfo() << "new connection";
    QTcpSocket* tempSocket = sslServer->nextPendingConnection();
    if(tempSocket == nullptr)
    {
        qInfo() << "It is nullptr";
        return;
    }
    this->sockets.push_back(tempSocket);

    connect(tempSocket, &QTcpSocket::readyRead, this, &Server::onNewClientMessage);
    connect(tempSocket, &QTcpSocket::disconnected, tempSocket, &QSslSocket::deleteLater);
    connect(tempSocket, &QTcpSocket::disconnected, [=](){
        this->sockets.removeOne(tempSocket);
        this->socketByUsername.remove(usernameBySocket[tempSocket]);
        this->usernameBySocket.remove(tempSocket);
        qInfo() << "disconected";
    });
}

void Server::onCallingToSomeone(QString to, QString from)
{
    QJsonObject toSend;
    toSend["message-type"] = "income-calling";
    toSend["from"] = from;
    toSend["to"] = to;
    qDebug() << "onCallingToSomeone" << toSend;
    //may i forgot to add something to toSend

    if(this->socketByUsername[to] != nullptr)
    {
        this->socketByUsername[to]->write(QJsonDocument(toSend).toJson());
        this->socketByUsername[to]->flush();
    }
}

void Server::onCallingEnds(QString to)
{
    QJsonObject toSend;
    toSend["message-type"] = "calling-end";

    if(this->socketByUsername[to] != nullptr)
    {
        this->socketByUsername[to]->write(QJsonDocument(toSend).toJson());
        this->socketByUsername[to]->flush();
    }
    qDebug() << "sent calling ended to another user"; ///CLEAR
}


