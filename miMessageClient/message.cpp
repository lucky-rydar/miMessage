#include "message.h"

Message::Message(int messageId, QString dateTime, QString fromUser, QString toUser, QString messageText, int groupId)
{
    this->messageId = messageId;
    this->dateTime.fromString(dateTime);
    this->fromUser = fromUser;
    this->toUser = toUser;
    this->massageText = messageText;
    this->groupId = groupId;


}
