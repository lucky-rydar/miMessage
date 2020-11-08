#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <client.h>
#include <addchatform.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_Registration_clicked();

    void on_Logining_clicked();

    void on_Send_clicked();

    void on_RegisterUserButton_clicked();

    void on_LoginUserButton_clicked();

    void on_AddChat_clicked();

private:
    Ui::MainWindow *ui;
    Client *client;

    QString *usernameToAdd;
};
#endif // MAINWINDOW_H
