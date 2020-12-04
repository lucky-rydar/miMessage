#include "audioserver.h"

AudioServer::AudioServer(QObject *parent) : QObject(parent)
{
    this->server = new QTcpServer(parent);
    port = 444;
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
