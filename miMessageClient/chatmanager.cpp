#include "chatmanager.h"

ChatManager::ChatManager(QObject *parent) : QObject(parent)
{

}

void ChatManager::addChat(Chat* chat)
{
    this->chatsList.push_back(chat);
}

void ChatManager::removeChat(QString chatName)
{

}

void ChatManager::addMessageTo(QString chatName)
{

}
