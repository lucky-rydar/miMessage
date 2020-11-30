#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <client.h>
#include <addchatform.h>
#include <chatmanager.h>
#include <message.h>
#include <notificationwin.h>

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
    void onNewMessage(Message message);
    void onLoginedUser(bool isLogined, QJsonObject obj); // here
    void on_QuitButton_clicked();
    void onChatButtonClicked();
    void onMessageSent(Message message);
    void onReceivedMessagesList(QJsonObject messages);

    void UploadChat(QString chatName);
    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    Client *client;
    AddChatForm *chatForm;
    NotificationWin* notificationWin;

    QString *usernameToAdd;
    ChatManager *chatManager;

    void addChatToList(bool isAdded, QString chatOrGroupName, int chatId);
};
#endif // MAINWINDOW_H
