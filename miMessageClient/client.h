#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork/QtNetwork>
#include <QtCore>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void registerUser(QString username, QString password);

    void onServerMessasge();
private:
    QTcpSocket *serverConnection;

signals:
    void Registered();
};

#endif // CLIENT_H
