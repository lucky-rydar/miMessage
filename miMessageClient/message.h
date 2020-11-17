#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>

class Message
{
public:
    Message(QString chatName, QString messageText = "", QString from = "", int messageId = -1);

    int messageId;
    QDateTime dateTime;
    QString massageText;
    QString chatName;
    QString from;
    //QString to;
};

#endif // MESSAGE_H
