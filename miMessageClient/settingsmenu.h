#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include <QWidget>
#include <QObject>
#include <QFile>
#include <QDebug>
#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>

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

    QString currentStyle;
    bool notificationEnabled;
private:
    Ui::SettingsMenu *ui;

    QFile *configFile;
signals:
    void styleChanged(QString styleName);

private slots:
    void save();
    void upload();
};

#endif // SETTINGSMENU_H
