#include "message.h"

Message::Message(QString chatName, QString messageText, int messageId)
{
    this->dateTime = QDateTime::currentDateTime();
    this->massageText = messageText;
    this->chatName = chatName;
    this->messageId = messageId;
}
