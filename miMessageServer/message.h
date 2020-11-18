#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>
class Message : public QObject
{

public:
    Message(QString chatName, QString messageText, QString dateTime, int messageId = -1, QObject* parent = nullptr);

    int messageId;
    QString dateTime;
    QString massageText;
    QString chatName;
    QString from;
signals:

};

#endif // MESSAGE_H
