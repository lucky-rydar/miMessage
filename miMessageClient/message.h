#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>

class Message
{
public:
    Message(QString chatName, QString messageText = "", int messageId = -1);

    int messageId;
    QDateTime dateTime;
    QString massageText;
    QString chatName;
};

#endif // MESSAGE_H
