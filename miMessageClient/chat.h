#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QtCore>
#include <QPushButton>

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QPushButton* btn, int chatId, QString chatName, QObject *parent = nullptr);
    QPushButton* chatInList;
    int chatId;
    QString chatName;
    QString chatOrGroup;

signals:

};

#endif // CHAT_H
