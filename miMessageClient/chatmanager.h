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
    void addMessageTo(QString chatName);
private:
    QList<Chat*> chatsList;
    QString currentChatName;
signals:

};

#endif // CHATMANAGER_H
