#ifndef CHATMEMBER_H
#define CHATMEMBER_H

#include <QObject>

class ChatMember : public QObject
{
    Q_OBJECT
public:
    explicit ChatMember(QObject *parent = nullptr);

    int chatId;
    QString username;
signals:

};

#endif // CHATMEMBER_H
