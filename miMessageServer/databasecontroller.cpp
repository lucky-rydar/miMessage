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
    QSqlQuery("CREATE TABLE 'Message' ('message_id' INTEGER,'date_time' TEXT,'message_text' TEXT,'from_user' TEXT,'to_user' TEXT,'group_name' TEXT,PRIMARY KEY('message_id'))", connection).exec(); // Message
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
