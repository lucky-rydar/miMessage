#include "databasecontroller.h"

DatabaseController::DatabaseController(QObject *parent) : QObject(parent)
{
    connection = QSqlDatabase::addDatabase("QSQLITE");
    connection.setDatabaseName("data.db");

    if(!connection.open())
        qCritical("Cant open database");

    QSqlQuery("CREATE TABLE 'Chat' ('id' INTEGER,'chat_name' TEXT,PRIMARY KEY('id'))", connection).exec(); // Chat
    QSqlQuery("CREATE TABLE 'ChatMember' ('chat_id' INTEGER,'username' TEXT)", connection).exec(); // ChatMember
    QSqlQuery("CREATE TABLE 'Group' ('id' INTEGER,'group_name' TEXT,PRIMARY KEY('id'))", connection).exec(); // Group
    QSqlQuery("CREATE TABLE 'GroupMember' ('group_id' INTEGER,'username' TEXT)", connection).exec(); // GroupMember
    QSqlQuery("CREATE TABLE 'Message' ('message_id' INTEGER,'date_time' TEXT,'message_text' TEXT,'from_user' TEXT,'to_user' "
              "TEXT,'group_name' TEXT,PRIMARY KEY('message_id'))", connection).exec(); // Message
    QSqlQuery("CREATE TABLE 'User' ('username' TEXT,'hashed_password' TEXT)", connection).exec(); // User
}

void DatabaseController::addUser(QString username, QString hashedPassword)
{
    if(!userExist(username))
        QSqlQuery("INSERT INTO User(username, hashed_password) VALUES(\'" + username + "\', \'" + hashedPassword + "\')", connection);
}

bool DatabaseController::userExist(QString username)
{
    return QSqlQuery("SELECT * FROM User WHERE username = '" + username + "';", connection).next();
}

bool DatabaseController::isRegistered(QString username, QString hashedPassword)
{
    return QSqlQuery("SELECT * FROM User WHERE username = '" + username + "' AND hashed_password = '" + hashedPassword + "';", connection).next();
}

bool DatabaseController::chatExist(QString username1, QString username2)
{
    QString chatName = usernamesToChatName(username1, username2);

    return QSqlQuery("SELECT * FROM Chat WHERE chat_name = '" + chatName + "'", connection).next();
}

bool DatabaseController::addChat(QString username1, QString username2)
{
    if(!QSqlQuery("SELECT * FROM User WHERE username = \'" + username1 + "\'", connection).next() || !QSqlQuery("SELECT * FROM User WHERE username = \'" + username2 + "\'", connection).next())
        return false;

    QString chatName = usernamesToChatName(username1, username2);

    QSqlQuery("INSERT INTO Chat(chat_name) VALUES(\'" + chatName + "\')", connection);

    int chatIdInt = chatIdByName(chatName);

    QSqlQuery("INSERT INTO ChatMember(chat_id, username) VALUES(\'" + QVariant(chatIdInt).toString() + "\', \'" + username1 + "\')", connection);
    QSqlQuery("INSERT INTO ChatMember(chat_id, username) VALUES(\'" + QVariant(chatIdInt).toString() + "\', \'" + username2 + "\')", connection);

    return true;
}

QString DatabaseController::usernamesToChatName(QString username1, QString username2)
{
    QString chatName;
    if(username1.toUtf8()[0] < username2.toUtf8()[0])
        chatName = username1 + "-" + username2;
    else
        chatName = username2 + "-" + username1;

    return chatName;
}

QList<QString> DatabaseController::usernamesFromChatName(QString chatName)
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

int DatabaseController::chatIdByName(QString chatName)
{
    QSqlQuery getId("SELECT * FROM Chat WHERE chat_name = \'" + chatName  + "\'", connection);
    getId.exec();
    getId.next();
    return getId.value(0).toInt();
}

QList<Chat> DatabaseController::getChatsByUsername(QString username)
{
    QList<Chat> chats;
    QSqlQuery chatIds("SELECT chat_id FROM ChatMember WHERE username = \'" + username + "\'", connection);
    chatIds.exec();

    int buffId = 0;
    while(chatIds.next())
    {
        buffId = chatIds.value(0).toInt();

        QSqlQuery chatName("SELECT chat_name FROM Chat WHERE id = " + QVariant(buffId).toString(), connection);

        chatName.first();
        QString buffChatName = chatName.value(0).toString();

        chats.push_back(Chat(buffId, buffChatName));
    }

    return chats;
}

void DatabaseController::addMessage(Message& message, QString fromUser, QString toUser, QString chatOrGroup, QString chatOrGroupName)
{
    if(chatOrGroup == "chat")
    {
        QSqlQuery addMessage("INSERT INTO Message(date_time, message_text, from_user, to_user) VALUES('" + message.dateTime + "', "
                                         "'" + message.massageText + "', '" + fromUser + "', '" + toUser + "')", connection);
        QSqlQuery getLastMessageId("SELECT message_id FROM Message WHERE from_user = '" + fromUser + "' AND to_user = '" + toUser + "' ORDER BY message_id DESC LIMIT 1", connection);
        getLastMessageId.exec();
        getLastMessageId.next();

        message.messageId = getLastMessageId.value(0).toInt();
    }
    else if(chatOrGroup == "group")
    {
        //TODO: add for chat almost the same
    }



}

QList<Message*> DatabaseController::getMessagesFor(QString chatOrGroupName, QString chatOrGroup)
{
    if(chatOrGroup == "chat")
    {
        QList<Message*> messages;

        auto usernames = DatabaseController::usernamesFromChatName(chatOrGroupName);
        QSqlQuery getMessages("SELECT * FROM Message WHERE from_user = '" + usernames[0] + "' AND to_user = '" + usernames[1] + "' OR from_user = '" + usernames[1] + "' AND to_user = '" +usernames[0] + "'", connection);
        getMessages.exec();

        while(getMessages.next())
        {
            Message *message = new Message(usernamesToChatName(getMessages.value(3).toString(), getMessages.value(4).toString()), getMessages.value(2).toString(), getMessages.value(1).toString(), getMessages.value(0).toInt());
            message->from = getMessages.value(3).toString();
            messages.append(message);
        }
        return messages;
    }
    else if(chatOrGroup == "group")
    {
        //TODO: add functionality for group too
    }

}
