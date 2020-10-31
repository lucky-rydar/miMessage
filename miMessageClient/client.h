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
    void loginUser(QString username, QString password);

    void onServerMessasge();
private:
    QTcpSocket *serverConnection;

    QString username;// these two variables must
    QString password;// be filled during logining
signals:
    void Registered(bool isRegistered);
    void Logined(bool isLogined);
};

#endif // CLIENT_H
