#ifndef AUDIOSERVER_H
#define AUDIOSERVER_H

#include <QObject>
#include <QtCore>
#include <QTcpServer>
#include <QTcpSocket>

class AudioServer : public QObject
{
    Q_OBJECT
public:
    explicit AudioServer(QObject *parent = nullptr);

    void run();
private:
    QTcpServer* server;
    int port;
    QMap<QString, QTcpSocket*> calling_users;

    void onNewConnection();

signals:
    void newCalling(QString toUsername);
    void accepted(QString fromUser, QString forUser);
    void declined(QString fromUser, QString forUser);
};

#endif // AUDIOSERVER_H
