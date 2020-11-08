#include "addchatform.h"
#include "ui_addchatform.h"

AddChatForm::AddChatForm(QString *usernameToAdd, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddChatForm)
{
    ui->setupUi(this);
    this->usernameToAdd = usernameToAdd;
}

AddChatForm::~AddChatForm()
{
    delete ui;
}

void AddChatForm::on_createChat_clicked()
{
    *this->usernameToAdd = ui->usernameToAdd->text();

    //tell the user if chat added
    this->close();
}
