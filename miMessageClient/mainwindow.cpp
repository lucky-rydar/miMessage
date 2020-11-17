#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new Client(this);
    usernameToAdd = new QString();
    chatManager = new ChatManager(this);

    ui->FormsAndMainMenu->setCurrentIndex(0);

    connect(client, &Client::newMessage, this, &MainWindow::onNewMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete usernameToAdd;
}

void MainWindow::on_Registration_clicked()
{
    ui->FormsAndMainMenu->setCurrentIndex(1);
}

void MainWindow::on_Logining_clicked()
{
    ui->FormsAndMainMenu->setCurrentIndex(0);
}

void MainWindow::on_Send_clicked()
{
    /*if(!ui->MessageTextEdit->text().isEmpty())
    {
        //client->sendMessageTo(ui->MessageTextEdit->text());
    }*/
}

void MainWindow::on_RegisterUserButton_clicked()
{
    if(ui->regPassword1->text().isEmpty() || ui->regPassword1->text().isEmpty() || ui->regUsername->text().isEmpty())
    {
        ui->regInfoLabel->setText("Smth empty.");
        QPalette palette = ui->regInfoLabel->palette();
        palette.setColor(ui->regInfoLabel->foregroundRole(), Qt::red);
        ui->regInfoLabel->setPalette(palette);
        return;
    }

    if(ui->regPassword1->text() != ui->regPassword2->text())
    {
        ui->regInfoLabel->setText("Incorect password.");
        QPalette palette = ui->regInfoLabel->palette();
        palette.setColor(ui->regInfoLabel->foregroundRole(), Qt::red);
        ui->regInfoLabel->setPalette(palette);
        return;
    }
    else
    {
        ui->regInfoLabel->setText("Trying to register you...");

        client->username = ui->regUsername->text(); //HERE!!!!

        QPalette palette = ui->regInfoLabel->palette();
        palette.setColor(ui->regInfoLabel->foregroundRole(), QColor(0, 200, 0));
        ui->regInfoLabel->setPalette(palette);

        client->registerUser(ui->regUsername->text(), ui->regPassword1->text());

        connect(client, &Client::Registered, [=](bool isRegistered){
            qDebug() << isRegistered;
            if(isRegistered)
            {
                ui->regInfoLabel->setText("Registered successful!");
                ui->regPassword1->clear();
                ui->regPassword2->clear();
                client->username.clear();
                ui->regUsername->clear();
                ui->FormsAndMainMenu->setCurrentIndex(0);
            }
            else
            {
                QPalette palette = ui->regInfoLabel->palette();
                palette.setColor(ui->regInfoLabel->foregroundRole(), Qt::red);
                ui->regInfoLabel->setPalette(palette);
                ui->regInfoLabel->setText("Cant register you");
            }
        });
    }


}

void MainWindow::on_LoginUserButton_clicked()
{
    if(ui->logUsername->text().isEmpty() || ui->logPassword->text().isEmpty())
    {
        ui->logInfoLabel->setText("Username or password line is empty.");
        QPalette palette = ui->logInfoLabel->palette();
        palette.setColor(ui->logInfoLabel->foregroundRole(), Qt::red);
        ui->logInfoLabel->setPalette(palette);
    }
    else
    {
        client->loginUser(ui->logUsername->text(), ui->logPassword->text());

        client->username = ui->logUsername->text();
        client->password = ui->logPassword->text();

        QPalette palette = ui->logInfoLabel->palette();
        ui->logInfoLabel->setText("Trying to login you");
        palette.setColor(ui->logInfoLabel->foregroundRole(), QColor(0, 200, 0));
        ui->logInfoLabel->setPalette(palette);

        connect(client, &Client::Logined, this, &MainWindow::onLoginedUser);
    }
}

void MainWindow::on_AddChat_clicked()
{
    chatForm = new AddChatForm(client, this);

    connect(chatForm, &AddChatForm::newChat, this, &MainWindow::addChatToList);

    chatForm->show();

    return;
}

void MainWindow::onNewMessage(QString chatName, QString messageText)
{
    // here should be reaction on new message
}

void MainWindow::onLoginedUser(bool isLogined, QJsonObject obj)
{
    QPalette palette = ui->logInfoLabel->palette();
    if(isLogined)
    {
        ui->logInfoLabel->setText("You are logined successfully");
        palette.setColor(ui->logInfoLabel->foregroundRole(), QColor(0, 200, 0));

        for (int i = 0; i < obj["chats-info"].toArray().size(); i++)
        {
            QPushButton *tempButton = new QPushButton(obj["chats-info"].toArray().at(i)["chat-name"].toString());
            this->chatsList.append(new Chat(tempButton, obj["chats-info"].toArray().at(i)["chat-id"].toInt(),
                    obj["chats-info"].toArray().at(i)["chat-name"].toString()));
            ui->ChatButtonsList->addWidget(tempButton);
        }

        ui->logPassword->clear();
        ui->logUsername->clear();
        ui->FormsAndMainMenu->setCurrentIndex(2);
    }
    else
    {
        ui->logInfoLabel->setText("Wrong username or password");
        palette.setColor(ui->logInfoLabel->foregroundRole(), Qt::red);

        client->username.clear();
        client->password.clear();

        ui->logPassword->clear();
        ui->logUsername->clear();
    }
    ui->logInfoLabel->setPalette(palette);
    disconnect(client, &Client::Logined, this, &MainWindow::onLoginedUser);
}

void MainWindow::addChatToList(QString chatName, int chatId)
{
    QPushButton *tempButton = new QPushButton(chatName);
    //this->chatsList.push_back(new Chat(tempButton, chatId, chatName, "chat", this));
    this->chatManager->addChat(new Chat(tempButton, chatId, chatName, "chat"));
    ui->ChatButtonsList->addWidget(tempButton);

    //TODO: connect new chat with some functionality

    disconnect(chatForm, &AddChatForm::newChat, this, &MainWindow::addChatToList);
}

void MainWindow::on_QuitButton_clicked()
{
    ui->FormsAndMainMenu->setCurrentIndex(0);
    client->username = "";
    client->password = "";
    ui->logInfoLabel->clear();

    while(ui->ChatButtonsList->count() > 0)
    {
        QLayoutItem *item = ui->ChatButtonsList->itemAt(0);
        ui->ChatButtonsList->removeItem(item);
        delete chatsList[0]->chatButton;
        chatsList.pop_front();
        delete item;
    }

}
