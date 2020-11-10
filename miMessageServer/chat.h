#ifndef CHAT_H
#define CHAT_H
#include <QtCore>
#include <QString>

class Chat
{
public:
    Chat(int id, QString name);

    int chatId;
    QString chatName;
};

#endif // CHAT_H
