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
    QTcpServer *tcpServer;
    DatabaseController *dbController;
    QVector<QTcpSocket*> clientsList;

    void newConnection();
signals:

};

#endif // SERVER_H
