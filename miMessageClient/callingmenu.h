#ifndef CALLINGMENU_H
#define CALLINGMENU_H

#include <QWidget>
#include <QDebug>
#include <client.h>

namespace Ui {
class CallingMenu;
}

class CallingMenu : public QWidget
{
    Q_OBJECT

public:
    explicit CallingMenu(Client *client, QWidget *parent = nullptr); // set the argument if you need to set parent's stylesheet
    ~CallingMenu();

    void setCallingUser(QString username);

private slots:
    void on_acceptCallingButton_clicked();
    void on_declineCallingButton_clicked();

private:
    Ui::CallingMenu *ui;
    Client* client;
};

#endif // CALLINGMENU_H
