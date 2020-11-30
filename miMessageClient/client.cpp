#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    this->serverConnection = new QSslSocket(this);

    this->serverAddress = QHostAddress("127.0.0.1");
    this->serverPort = 333;

    serverConnection->connectToHost(serverAddress, serverPort);
    serverConnection->setProtocol(QSsl::TlsV1_2);
    connect(serverConnection, &QSslSocket::readyRead, this, &Client::onServerMessasge);
}

void Client::registerUser(QString username, QString password)
{
    QJsonObject toSend;
    toSend["message-type"] = "registration";
    toSend["username"] = username;
    toSend["password"] = password;

    serverConnection->write(QJsonDocument(toSend).toJson());
    serverConnection->flush();
}

void Client::loginUser(QString username, QString password)
{
    QJsonObject toSend;
    toSend["message-type"] = "logining";
    toSend["username"] = username;
    toSend["password"] = password;

    serverConnection->write(QJsonDocument(toSend).toJson());
    serverConnection->flush();
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

    serverConnection->write(QJsonDocument(toSend).toJson());
    serverConnection->flush();
}

void Client::sendMessageTo(Message message, Chat *chat)
{
    QJsonObject toSend;
    toSend["username"] = this->username;
    toSend["password"] = this->password;
    toSend["message-type"] = "sending-message";
    toSend["message-text"] = message.massageText;
    toSend["chat-or-group"] = chat->chatOrGroup;
    toSend["chat-or-group-name"] = chat->chatName;
    toSend["sending-date-time"] = message.dateTime.toString();

    QList<QString> usernames = Client::usernamesFromChatName(chat->chatName);
    if(usernames[0] == username)
    {
        toSend["from-user"] = usernames[0];
        toSend["to-user"] = usernames[1];
    }
    else
    {
        toSend["from-user"] = usernames[1];
        toSend["to-user"] = usernames[0];
    }

    qDebug() << toSend;

    serverConnection->write(QJsonDocument(toSend).toJson());
    serverConnection->flush();
}

void Client::getMessagesFor(Chat *chat)
{
    QJsonObject toSend;
    toSend["username"] = this->username;
    toSend["password"] = this->password;
    toSend["message-type"] = "messages-request";
    toSend["chat-or-group"] = chat->chatOrGroup;
    toSend["chat-or-group-name"] = chat->chatName;

    qDebug() << toSend;

    serverConnection->write(QJsonDocument(toSend).toJson());
    serverConnection->flush();
}

void Client::onServerMessasge()
{
    QByteArray buff = serverConnection->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(buff);
    QJsonObject obj = doc.object();

    qDebug() << "Received message" << obj;
    if(obj["message-type"] == "reg-status")
        emit Registered(obj["registered"].toBool());
    else if(obj["message-type"] == "login-status")
            emit Logined(obj["logined"].toBool(), obj);
    else if(obj["message-type"] == "adding-chat-group-status")
        emit AddedNewChat(obj["is-added"].toBool(), obj["chat-or-group-name"].toString(), obj["chat-id"].toInt());
    else if(obj["message-type"] == "new-message")
        emit newMessage(Message(obj["chat-name"].toString(), obj["message-text"].toString(), obj["from-user"].toString(), obj["message-id"].toInt(), QDateTime::fromString(obj["date-time"].toString())));
    else if(obj["message-type"] == "sent-message-status")
        emit messageSent(Message(obj["chat-name"].toString(), obj["message-text"].toString(), obj["from-user"].toString(), obj["message-id"].toInt()));
    else if(obj["message-type"] == "message-list-for")
        emit receivedMessagesList(obj);
}

void Client::disconectFromHost()
{
    this->serverConnection->disconnectFromHost();
}

QList<QString> Client::usernamesFromChatName(QString chatName)
{
    std::regex rx("([A-Za-z0-9]+)-([A-Za-z0-9]+)");
    std::string to_parse = chatName.toStdString();
    std::smatch parsed;
    std::regex_match(to_parse, parsed, rx);
    QList<QString> result;

    result.push_back(QString::fromStdString(parsed[1]));
    result.push_back(QString::fromStdString(parsed[2]));

    return result;
}
