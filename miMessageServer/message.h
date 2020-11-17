#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>
class Message : public QObject
{
    Q_OBJECT
public:
    //explicit Message(QObject *parent = nullptr);
    Message(QString chatName, QString messageText, QDateTime dateTime, int messageId, QObject* parent = nullptr);

    int messageId;
    QDateTime dateTime;
    QString massageText;
    QString chatName;

signals:

};

#endif // MESSAGE_H
