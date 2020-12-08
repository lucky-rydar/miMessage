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
            qDebug() << "The calling was accepted";

            emit this->accepted(usersData["calling-user"].toString());

            this->alreadyConnectedByUsername.insert(usersData["calling-user"].toString(), calling_users[usersData["calling-user"].toString()]);
            this->alreadyConnectedByUsername.insert(usersData["called-user"].toString(), sock);
            this->calling_users.remove(usersData["calling-user"].toString());

            connect(this->alreadyConnectedByUsername[usersData["calling-user"].toString()], &QTcpSocket::disconnected, [=](){
                this->alreadyConnectedByUsername.remove(usersData["calling-user"].toString());

                if(this->alreadyConnectedByUsername.contains(usersData["called-user"].toString()))
                    emit this->disconnected(usersData["called-user"].toString());
            });

            connect(this->alreadyConnectedByUsername[usersData["called-user"].toString()], &QTcpSocket::disconnected, [=](){
                this->alreadyConnectedByUsername.remove(usersData["called-user"].toString());

                if(this->alreadyConnectedByUsername.contains(usersData["calling-user"].toString()))
                    emit this->disconnected(usersData["calling-user"].toString());
            });

            connect(sock, &QTcpSocket::readyRead, [=](){
                auto received = sock->readAll();
                this->alreadyConnectedByUsername[usersData["calling-user"].toString()]->write(received);
                this->alreadyConnectedByUsername[usersData["calling-user"].toString()]->flush();
            });

            connect(this->alreadyConnectedByUsername[usersData["calling-user"].toString()], &QTcpSocket::readyRead, [=](){
                auto received = this->alreadyConnectedByUsername[usersData["calling-user"].toString()]->readAll();
                sock->write(received);
                sock->flush();
            });

            qDebug() << "two clients connected";
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
