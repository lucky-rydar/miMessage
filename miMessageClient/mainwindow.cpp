#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("miMessage");

    client = new Client(this);
    usernameToAdd = new QString();
    chatManager = new ChatManager(this);

    ui->FormsAndMainMenu->setCurrentIndex(0);

    connect(client, &Client::newMessage, this, &MainWindow::onNewMessage);
    connect(client, &Client::AddedNewChat, this, &MainWindow::addChatToList);

    notificationWin = new NotificationWin();
    settingsMenu = new SettingsMenu(parent);

    connect(settingsMenu, &SettingsMenu::styleChanged, [=](QString styleName){
        QString path = ":/styles/" + styleName + ".qss";
        QFile styleSheet(path);
        styleSheet.open(QFile::ReadOnly);
        this->setStyleSheet(styleSheet.readAll());
    });
    settingsMenu->callDefaultStyle();

    ui->disconnectButton->setVisible(false);

    connect(client, &Client::incomeCalling, this, &MainWindow::onIncomeCalling);

    callingMenu = new CallingMenu(this->client, this);

    connect(callingMenu, &CallingMenu::acceptedButtonPressed, client, &Client::bindAudioFromMicro);
    connect(callingMenu, &CallingMenu::acceptedButtonPressed, client, &Client::bindAudioFromSocket);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete usernameToAdd;
    delete notificationWin;
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
    if(!ui->MessageTextEdit->text().isEmpty() && !chatManager->currentChatName.isEmpty())
    {
        client->sendMessageTo(Message(chatManager->currentChatName, ui->MessageTextEdit->text()), chatManager->getChatByName(chatManager->currentChatName));

        connect(client, &Client::messageSent, this, &MainWindow::onMessageSent);
    }
    ui->MessageTextEdit->clear();
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

        client->username = ui->regUsername->text();

        QPalette palette = ui->regInfoLabel->palette();
        palette.setColor(ui->regInfoLabel->foregroundRole(), QColor(0, 200, 0));
        ui->regInfoLabel->setPalette(palette);

        client->registerUser(ui->regUsername->text(), ui->regPassword1->text());

        connect(client, &Client::Registered, [=](bool isRegistered){
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

    chatForm->show();

    return;
}

void MainWindow::onNewMessage(Message message)
{
    chatManager->addMessage(message);
    if(chatManager->currentChatName == message.chatName)
        UploadChat(message.chatName);

    if(!this->isActiveWindow() && settingsMenu->notificationEnabled)
    {
        notificationWin->setPopupText("[" + message.dateTime.toString("hh:mm:ss") + "]<" + message.from + ">: " + message.massageText);

        notificationWin->show();
    }
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
            chatManager->addChat(new Chat(tempButton, obj["chats-info"].toArray().at(i)["chat-id"].toInt(),
                    obj["chats-info"].toArray().at(i)["chat-name"].toString()));
            ui->ChatButtonsList->addWidget(tempButton);
            connect(tempButton, &QPushButton::clicked, this, &MainWindow::onChatButtonClicked);
        }
        ui->currentUsernameLabel->setText(ui->logUsername->text());

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

void MainWindow::addChatToList(bool isAdded, QString chatOrGroupName, int chatId)
{
    if(isAdded)
    {
        QPushButton *tempButton = new QPushButton(chatOrGroupName);
        this->chatManager->addChat(new Chat(tempButton, chatId, chatOrGroupName, "chat"));
        ui->ChatButtonsList->addWidget(tempButton);

        connect(tempButton, &QPushButton::clicked, this, &MainWindow::onChatButtonClicked);
    }

}

void MainWindow::on_QuitButton_clicked()
{
    ui->FormsAndMainMenu->setCurrentIndex(0);
    client->username = "";
    client->password = "";
    ui->logInfoLabel->clear();
    chatManager->currentChatName.clear();

    while(ui->ChatButtonsList->count() > 0)
    {
        QLayoutItem *item = ui->ChatButtonsList->itemAt(0);
        ui->ChatButtonsList->removeItem(item);
        delete this->chatManager->chatsList[0]->chatButton;
        this->chatManager->chatsList.pop_front();
        delete item;
    }
    ui->MessagesArea->clear();
    ui->currentChatLabel->setText("-");
}

void MainWindow::onChatButtonClicked()
{
    QPushButton* chatButton = qobject_cast<QPushButton*>(sender());
    this->chatManager->currentChatName = chatButton->text();

    ui->currentChatLabel->setText(this->chatManager->currentChatName);
    client->getMessagesFor(chatManager->getChatByName(chatButton->text()));

    connect(client, &Client::receivedMessagesList, this, &MainWindow::onReceivedMessagesList);

    UploadChat(chatButton->text());
}

void MainWindow::onMessageSent(Message message)
{
    chatManager->addMessage(message);

    UploadChat(message.chatName);
    disconnect(client, &Client::messageSent, this, &MainWindow::onMessageSent);
}

void MainWindow::onReceivedMessagesList(QJsonObject messages)
{
    auto chat = chatManager->getChatByName(messages["chat-or-group-name"].toString());
    chat->messages.clear();

    auto messagesArray = messages["messages-list"].toArray();
    for (int i = 0; i < messagesArray.size(); i++)
    {
        Message loadedMessage(messages["chat-or-group-name"].toString(), messagesArray.at(i)["message-text"].toString(),
                        messagesArray.at(i)["from-user"].toString(), messagesArray.at(i)["message-id"].toInt());
        loadedMessage.dateTime = QDateTime::fromString(messagesArray.at(i)["date-time"].toString());
        chat->messages.push_back(loadedMessage);
    }

    UploadChat(messages["chat-or-group-name"].toString());
    disconnect(client, &Client::receivedMessagesList, this, &MainWindow::onReceivedMessagesList);
}

void MainWindow::onIncomeCalling(QString from)
{
    callingMenu->setCallingUser(from);
    callingMenu->show();
}

void MainWindow::UploadChat(QString chatName)
{
    ui->MessagesArea->clear();

    Chat *chat = chatManager->getChatByName(chatName);
    if(chat != nullptr)
    {
        QString buff;

        for (int i = 0; i < chat->messages.size(); i++)
        {
            QString formatedMessage = "[" + chat->messages[i].dateTime.toString("hh:mm:ss") + "]<" + chat->messages[i].from + ">: " + chat->messages[i].massageText + "\n";
            buff += formatedMessage;
        }
        ui->MessagesArea->setText(buff);
    }

}

void MainWindow::on_SettingsButton_clicked()
{
    if(settingsMenu->isVisible())
        settingsMenu->close();
    else
        settingsMenu->show();
}

void MainWindow::on_VideocallButton_clicked()
{

}

void MainWindow::on_CallButton_clicked()
{
    if(!chatManager->currentChatName.isEmpty())
        client->makeAudioConnection(chatManager->currentChatName, client->username);
}

