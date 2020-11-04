#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

class Message : public QObject
{
    Q_OBJECT
public:
    explicit Message(QObject *parent = nullptr);

    int messageId;
    QDateTime *dateTime;
    QString messageText;
    QString fromUser;
    QString toUser;
    int groupId; // if exist

signals:

};

#endif // MESSAGE_H
