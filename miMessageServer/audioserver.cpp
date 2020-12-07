#include "audioserver.h"

AudioServer::AudioServer(QObject *parent) : QObject(parent)
{
    this->server = new QTcpServer(parent);
    port = 444;

    connect(server, &QTcpServer::newConnection, this, &AudioServer::onNewConnection);
}

void AudioServer::run()
{
    if(!this->server->listen(QHostAddress::AnyIPv4, this->port))
    {
        qCritical("Cant run audio server");
        return;
    }
    qInfo("Audio server started successfully...");
}

void AudioServer::onNewConnection()
{
    QTcpSocket* sock = server->nextPendingConnection();
    connect(sock, &QTcpSocket::disconnected, [=](){
        qDebug() << "disconnected from audio server";
    });
    sock->waitForReadyRead();

    qDebug() << "New Audio connection";

    QByteArray buff;
    buff = sock->readAll();
    QJsonObject usersData = QJsonDocument::fromJson(buff).object();
    qDebug() << usersData;

    if(usersData["i-am"].toString() == "calling-user")
    {
        this->calling_users.insert(usersData["calling-username"].toString(), sock);
        //TODO: send message to another user that we are calling to him/her (using signal of course)


        emit newCalling(usersData["called-username"].toString(), usersData["calling-username"].toString());
        qDebug() << "Connectected new calling user";
    }
    else if(usersData["i-am"].toString() == "called-user")
    {

        if(usersData["accept-decline"].toString() == "accept")
        {
            //TODO: tell calling user that calling was accepted and make connection with another socket
            qDebug() << "The calling was accepted";

            connect(sock, &QTcpSocket::readyRead, [=](){
                auto received = sock->readAll();
                calling_users[usersData["calling-user"].toString()]->write(received);
                calling_users[usersData["calling-user"].toString()]->flush();
                //qDebug() << "sent user 1";
            });
            connect(calling_users[usersData["calling-user"].toString()], &QTcpSocket::readyRead, [=](){
                auto received = calling_users[usersData["calling-user"].toString()]->readAll();
                sock->write(received);
                sock->flush();
                //qDebug() << "sent user 2";
            });
            emit this->accepted(usersData["calling-user"].toString());
            qDebug() << "two clients connected";

            //this->calling_users.remove(usersData["calling-user"].toString());
        }
        else if(usersData["accept-decline"].toString() == "decline")
        {
            //TODO: tell calling user to that another user declined calling
            qInfo() << "Calling was declined and calling user diconnected";

            emit declined(usersData["calling-user"].toString());
            this->calling_users.remove(usersData["calling-user"].toString());
        }
    }

}
