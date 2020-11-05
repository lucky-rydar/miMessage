#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork/QtNetwork>
#include <databasecontroller.h>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void run();

private:
    QUdpSocket* udpServer;
    DatabaseController *dbController;

    void onNewClientMessage();
signals:

};

#endif // SERVER_H
