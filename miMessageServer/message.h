#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>
class Message : public QObject
{
    Q_OBJECT
public:
    //explicit Message(QObject *parent = nullptr);
    Message(QString chatName, QString messageText, QString dateTime, int messageId = -1, QObject* parent = nullptr);

    int messageId;
    QString dateTime;
    QString massageText;
    QString chatName;

signals:

};

#endif // MESSAGE_H
