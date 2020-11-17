#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>

class Message
{
public:
    Message(int messageId, QString dateTime, QString fromUser, QString toUser, QString messageText = "", int groupId = -1);

    int messageId;
    QDateTime dateTime;
    QString massageText;
    QString fromUser;
    QString toUser;
    int groupId;
};

#endif // MESSAGE_H
