#include "message.h"

Message::Message(QString chatName, QString messageText, QString from, int messageId, QDateTime dateTime)
{
    this->dateTime = dateTime;
    this->massageText = messageText;
    this->chatName = chatName;
    this->messageId = messageId;
    this->from = from;
}
