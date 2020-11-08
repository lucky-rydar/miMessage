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
    if(ui->usernameToAdd->text().isEmpty())
        return;

    //tell the user if chat added
    this->close();
}
