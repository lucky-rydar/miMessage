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
    void addNewChatGroup(QString chatOrGroupName, QString chatOrGroup);
    void sendMessageTo(QString messageText, QString chatName, QString chatOrGroup);

    void onServerMessasge();

    QString username;// these two variables must
    QString password;// be filled during logining
private:
    QUdpSocket *serverConnection;
    QHostAddress serverAddress;
    int serverPort;
signals:
    void Registered(bool isRegistered);
    void Logined(bool isLogined, QJsonObject received);
    void AddedNewChat(bool isAdded, QString chatOrGroupName, int chatId);
    void newMessage(QString chatName, QString text);
};

#endif // CLIENT_H
