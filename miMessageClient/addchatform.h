#ifndef ADDCHATFORM_H
#define ADDCHATFORM_H

#include <QDialog>
#include <client.h>

namespace Ui {
class AddChatForm;
}

class AddChatForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddChatForm(Client *client, QWidget *parent = nullptr);
    ~AddChatForm();

private slots:
    void on_createChat_clicked();

signals:
    void newChat(QString chatName, int chatId);

private:
    Ui::AddChatForm *ui;
    Client *client;
};

#endif // ADDCHATFORM_H
