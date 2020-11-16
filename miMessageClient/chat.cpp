#include "chat.h"

Chat::Chat(QPushButton* btn, int chatId, QString chatName, QString chatOrGroup, QObject *parent) : QObject(parent)
{
    this->chatButton = btn;
    this->chatId = chatId;
    this->chatName = chatName;
    this->chatOrGroup = chatOrGroup;
}
