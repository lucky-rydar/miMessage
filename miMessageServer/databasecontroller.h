#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include <QtSql/QtSql>
#include <chat.h>
#include <message.h>

class DatabaseController : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseController(QObject *parent = nullptr);

    void addUser(QString username, QString hashedPassword);
    bool userExist(QString username);
    bool isRegistered(QString username, QString hashedPassword);
    bool chatExist(QString username1, QString username2);
    bool addChat(QString username1, QString username2);
    int chatIdByName(QString chatName);
    QList<Chat> getChatsByUsername(QString username);
    void addMessage(Message message);

    QString usernamesToChatName(QString username1, QString username2);

private:


    QSqlDatabase connection;
signals:

};

#endif // DATABASECONTROLLER_H
