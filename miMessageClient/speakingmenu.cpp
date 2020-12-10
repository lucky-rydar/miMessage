#include "speakingmenu.h"
#include "ui_speakingmenu.h"

SpeakingMenu::SpeakingMenu(QAudioInput *audioSender, QString username, Client* client, QWidget *parent) :
    QWidget(nullptr), //nullptr because window delets itself after closing
    ui(new Ui::SpeakingMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("Speaking menu");
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowFlags(Qt::SubWindow);

    if(parent != nullptr)
        setStyleSheet(parent->styleSheet());

    this->audioSender = audioSender;

    ui->soundVolume->setMinimum(0);
    ui->soundVolume->setMaximum(100);
    ui->soundVolume->setSingleStep(1);

    ui->username->setText(username);

    this->isMicroOn = true;

    this->client = client;

    ui->endCall->setStyleSheet("background: tomato;"
                               "color: #FFF;");

    connect(ui->endCall, &QPushButton::clicked, this, &SpeakingMenu::on_endCall_clicked);

    ui->soundVolume->setSliderPosition(100);
}

SpeakingMenu::~SpeakingMenu()
{
    delete ui;
}

void SpeakingMenu::on_endCall_clicked()
{
    //emit slot that will disconnect audio socket in MainWindow
    client->audioConnection->disconnectFromHost();
    emit endCall();
    this->close();
}

void SpeakingMenu::on_microModeButton_clicked()
{
    if(isMicroOn)
    {
        this->audioSender->suspend();
        this->isMicroOn = false;
        ui->microModeButton->setText("Turn on micro");
    }
    else
    {
        this->audioSender->resume();
        this->isMicroOn = true;
        ui->microModeButton->setText("Turn off micro");
    }
}

void SpeakingMenu::on_soundVolume_valueChanged(int value)
{
    this->audioSender->setVolume(value);
}
