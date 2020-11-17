#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QtCore>
#include <QPushButton>
#include <message.h>

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QPushButton* btn, int chatId, QString chatName, QString chatOrGroup = "chat", QObject *parent = nullptr);
    QPushButton* chatButton;
    int chatId;
    QString chatName;
    QString chatOrGroup;

    QList<Message> messages;
signals:

};

#endif // CHAT_H
