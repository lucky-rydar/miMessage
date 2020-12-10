#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork/QtNetwork>
#include <QtCore>
#include <message.h>
#include <chat.h>
#include <regex>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void tryConnectToserver();

    void registerUser(QString username, QString password);
    void loginUser(QString username, QString password);
    void addNewChatGroup(QString chatOrGroupName, QString chatOrGroup);
    void sendMessageTo(Message message, Chat *chat);
    void getMessagesFor(Chat *chat);
    void makeAudioConnection(QString chatName, QString who);
    void callingAnswer(QString acceptOrDecline, QString acceptFor);

    void onServerMessasge();

    void disconectFromHost();

    static QList<QString> usernamesFromChatName(QString chatName);

    QString username;// these two variables must
    QString password;// be filled during logining

    QString speakingWith;

    QTcpSocket *audioConnection;
    QSslSocket *serverConnection;

    QAudioInput *audioInput;
    QAudioOutput *audioOutput;
private:
    QHostAddress serverAddress;
    int serverPort;

    QAudioFormat inputOutputFormat;
    int audioBufferSize;
signals:
    void Registered(bool isRegistered);
    void Logined(bool isLogined, QJsonObject received);
    void AddedNewChat(bool isAdded, QString chatOrGroupName, int chatId);
    void newMessage(Message message);
    void messageSent(Message message);
    void receivedMessagesList(QJsonObject messages);
    void incomeCalling(QString from);
    void callingDeclined();
    void callingAccepted();
    void callingEnd();
public slots:
    void bindAudioFromMicro();
    void bindAudioFromSocket();
    void stopAudio();
};

#endif // CLIENT_H
