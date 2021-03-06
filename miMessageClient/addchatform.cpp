#include "addchatform.h"
#include "ui_addchatform.h"

AddChatForm::AddChatForm(Client *client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChatForm)
{
    this->client = client;
    ui->setupUi(this);
    this->setWindowTitle("Add chat menu");
}

AddChatForm::~AddChatForm()
{
    delete ui;
}

void AddChatForm::on_createChat_clicked()
{
    if(ui->chatOrGroupNameEdit->text().isEmpty() || ui->chatOrGroupNameEdit->text() == client->username)
        return;

    QString chatOrGroup;
    if(ui->GroupRadio->isChecked()) // TODO: rewrite
        chatOrGroup = "group";
    else if(ui->ChatRadio->isChecked())
        chatOrGroup = "chat";

    client->addNewChatGroup(ui->chatOrGroupNameEdit->text(), chatOrGroup);

    this->close();
}
