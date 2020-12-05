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
    sock->waitForReadyRead();

    QByteArray buff;
    buff = sock->readAll();
    QJsonObject usersData = QJsonDocument::fromJson(buff).object();
    qDebug() << usersData;

    if(usersData["i-am"].toString() == "calling-user")
    {
        this->calling_users.insert(usersData["calling-username"].toString(), sock);
        //TODO: send message to another user that we are calling to him/her (using signal of course)


        emit newCalling(usersData["called-username"].toString(), usersData["calling-user"].toString());
        qDebug() << "Connectected new calling user";
    }
    else if(usersData["i-am"].toString() == "called-user")
    {
        //TODO: make accepting and declining
        if(usersData["accept-decline"].toString() == "accept")
        {
            //TODO: tell calling user that calling was accepted and make connection with another socket

            this->calling_users.remove(usersData["calling-user"].toString());
        }
        else if(usersData["insert-decline"].toString() == "decline")
        {
            //TODO: tell calling user to that another user declined calling

            this->calling_users.remove(usersData["calling-user"].toString());
        }
    }

}
