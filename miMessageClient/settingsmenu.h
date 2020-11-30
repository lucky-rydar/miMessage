#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include <QWidget>
#include <QObject>
#include <QFile>
#include <QMainWindow>

namespace Ui {
class SettingsMenu;
}

class SettingsMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsMenu(QWidget *parent = nullptr);
    ~SettingsMenu();

    void callDefaultStyle();
private:
    Ui::SettingsMenu *ui;

    QString currentStyle;
    bool notificationEnabled;

signals:
    void styleChanged(QString styleName);

};

#endif // SETTINGSMENU_H
