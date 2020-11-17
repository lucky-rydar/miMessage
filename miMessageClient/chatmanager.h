#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QObject>
#include <chat.h>

class ChatManager : public QObject
{
    Q_OBJECT
public:
    explicit ChatManager(QObject *parent = nullptr);

    void addChat(Chat* chat);
    void removeChat(QString chatName);
    void addMessageTo(QString chatName, Message message);
    Chat* getChatByName(QString chatName);

    QList<Chat*> chatsList;
    QString currentChatName;
private:


signals:

};


#endif // CHATMANAGER_H
