#ifndef SPEAKINGMENU_H
#define SPEAKINGMENU_H

#include <QWidget>
#include <QAudioInput>

namespace Ui {
class SpeakingMenu;
}

class SpeakingMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SpeakingMenu(QAudioInput *audioSender, QString username, QWidget *parent = nullptr);
    ~SpeakingMenu();

signals:
    void endCall();

private slots:
    void on_endCall_clicked();
    void on_microModeButton_clicked();
    void on_soundVolume_actionTriggered(int action);

private:
    Ui::SpeakingMenu *ui;
    QAudioInput* audioSender;

    bool isMicroOn;
};

#endif // SPEAKINGMENU_H
