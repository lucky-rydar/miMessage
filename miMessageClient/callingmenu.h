#ifndef CALLINGMENU_H
#define CALLINGMENU_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class CallingMenu;
}

class CallingMenu : public QWidget
{
    Q_OBJECT

public:
    explicit CallingMenu(QWidget *parent = nullptr); // set the argument if you need to set parent's stylesheet
    ~CallingMenu();

    void setCallingUser(QString username);

private slots:
    void on_acceptCallingButton_clicked();
    void on_declineCallingButton_clicked();

private:
    Ui::CallingMenu *ui;
};

#endif // CALLINGMENU_H
