#include "chat.h"

Chat::Chat(QPushButton* btn, int chatId, QString chatName, QObject *parent) : QObject(parent)
{
    this->chatInList = btn;
    this->chatId = chatId;
    this->chatName = chatName;
    this->chatOrGroup = chatOrGroup;
}
