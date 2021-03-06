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
    QMap<QString, QTcpSocket*> alreadyConnectedByUsername;

private slots:
    void onNewConnection();

signals:
    void newCalling(QString toUser, QString fromUser);
    void accepted(QString fromUser);
    void declined(QString fromUser);
    void disconnected(QString sendTo);

};

#endif // AUDIOSERVER_H
