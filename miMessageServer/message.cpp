#include "message.h"

Message::Message(QString chatName, QString messageText, QDateTime dateTime, int messageId, QObject* parent) : QObject(parent)
{
    this->dateTime = dateTime;
    this->chatName = chatName;
    this->massageText = messageText;
    this->messageId = messageId;
}
