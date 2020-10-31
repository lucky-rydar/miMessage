#include "databasecontroller.h"

DatabaseController::DatabaseController(QObject *parent) : QObject(parent)
{
    connection = QSqlDatabase::addDatabase("QSQLITE");
    connection.setDatabaseName("data.db");

    if(!connection.open())
        qCritical("Cant open database");
    QSqlQuery("CREATE TABLE \"groups_members\" (\"group_name\" TEXT, \"username\" TEXT);", connection).exec();
    QSqlQuery("CREATE TABLE \"messages\" (\"message_id\" INTEGER, \"date_time\" TEXT, \"message_text\" TEXT,\"from_user\" TEXT, \"to_user\"	TEXT, \"group_name\" TEXT, PRIMARY KEY(\"message_id\"));", connection).exec();
    QSqlQuery("CREATE TABLE \"users\" (\"id\" INTEGER, \"username\"	TEXT, \"hashed_password\" TEXT, PRIMARY KEY(\"id\"))", connection).exec();
}

void DatabaseController::addUser(QString username, QString hashedPassword)
{
    if(!userExist(username))
        QSqlQuery("INSERT INTO users(username, hashed_password) VALUES(\'" + username + "\', \'" + hashedPassword + "\')", connection);
}

bool DatabaseController::userExist(QString username)
{
    return QSqlQuery("SELECT * FROM users WHERE username = '" + username + "';", connection).next();
}

bool DatabaseController::isRegistered(QString username, QString hashedPassword)
{
    return QSqlQuery("SELECT * FROM users WHERE username = '" + username + "' AND hashed_password = '" + hashedPassword + "';", connection).next();
}
