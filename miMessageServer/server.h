#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QMap>
#include <QtNetwork/QtNetwork>
#include <databasecontroller.h>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void run();

private:
    QTcpServer* sslServer;
    DatabaseController *dbController;
    int port;

    QList<QTcpSocket*> sockets;
    QMap<QString, QTcpSocket*> socketByUsername;
    QMap<QTcpSocket*, QString> usernameBySocket;

    void onNewClientMessage();
    void onNewConnection();

signals:

};

#endif // SERVER_H
