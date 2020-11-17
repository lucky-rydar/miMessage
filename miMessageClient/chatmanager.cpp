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
    for(int i = 0; i < chatsList.size(); i++)
    {
        if(chatsList[i]->chatName == chatName)
        {
            chatsList.removeAt(i);
            break;
        }
    }
}

void ChatManager::addMessageTo(QString chatName, Message message)
{

}

Chat *ChatManager::getChatByName(QString chatName)
{
    for(int i = 0; i < this->chatsList.size(); i++)
    {
        if(chatsList[i]->chatName == chatName)
            return chatsList[i];
    }
    return nullptr;
}
