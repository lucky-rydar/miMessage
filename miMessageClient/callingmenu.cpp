#include "callingmenu.h"
#include "ui_callingmenu.h"

CallingMenu::CallingMenu(Client *client, QWidget *parent) :
    QWidget(nullptr), //<== bad practic but I have no idea how to fix this in another way
    ui(new Ui::CallingMenu)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlags(Qt::SubWindow);

    if(parent != nullptr)
        setStyleSheet(parent->styleSheet());

    ui->acceptCallingButton->setStyleSheet("background: #44CC44;"
                                           "color: #FFF;");
    ui->declineCallingButton->setStyleSheet("background: tomato;"
                                            "color: #FFF;");

    this->client = client;
}

CallingMenu::~CallingMenu()
{
    delete ui;
}

void CallingMenu::setCallingUser(QString username)
{
    ui->usernameLabel->setText(username);
}

void CallingMenu::on_acceptCallingButton_clicked()
{
    //send the message to server that you declined the calling
    this->close();
}

void CallingMenu::on_declineCallingButton_clicked()
{
    //send the message to server that you accepted the calling and emit signal for MainWindow to make new connection
    this->close();
}
