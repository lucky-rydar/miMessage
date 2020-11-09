#include "addchatform.h"
#include "ui_addchatform.h"

AddChatForm::AddChatForm(Client *client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChatForm)
{
    this->client = client;
    ui->setupUi(this);
}

AddChatForm::~AddChatForm()
{
    delete ui;
}

void AddChatForm::on_createChat_clicked()
{
    if(ui->chatOrGroupNameEdit->text().isEmpty())
        return;

    QString chatOrGroup;
    if(ui->GroupRadio->isEnabled())
        chatOrGroup = "group";
    else if(ui->ChatRadio->isEnabled())
        chatOrGroup = "chat";

    client->addNewChatGroup(ui->chatOrGroupNameEdit->text(), chatOrGroup);
    connect(client, &Client::AddedNewChat, [=](bool isAdded, QString chatOrGroupName, QString chatOrGroup, int chatId){
        //TODO: adding chat to list of all chats and connect it with some functionality
        if(isAdded)
        {
            emit this->newChat(chatOrGroupName, chatId);

        }

    });
    //tell the user if chat added
    this->close();
}
