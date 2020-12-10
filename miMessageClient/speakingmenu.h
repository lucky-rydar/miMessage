#ifndef SPEAKINGMENU_H
#define SPEAKINGMENU_H

#include <QWidget>
#include <QAudioInput>
#include <client.h>

namespace Ui {
class SpeakingMenu;
}

class SpeakingMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SpeakingMenu(QAudioInput *audioSender, QString username, Client* client, QWidget *parent = nullptr);
    ~SpeakingMenu();

    void on_endCall_clicked();
signals:
    void endCall();

private slots:

    void on_microModeButton_clicked();
    void on_soundVolume_actionTriggered(int action);

private:
    Ui::SpeakingMenu *ui;
    QAudioInput* audioSender;
    Client* client;
    bool isMicroOn;
};

#endif // SPEAKINGMENU_H
