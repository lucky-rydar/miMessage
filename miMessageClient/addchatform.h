#ifndef ADDCHATFORM_H
#define ADDCHATFORM_H

#include <QDialog>

namespace Ui {
class AddChatForm;
}

class AddChatForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddChatForm(QString* usernameToAdd = nullptr, QWidget *parent = nullptr);
    ~AddChatForm();

private slots:
    void on_createChat_clicked();

private:
    Ui::AddChatForm *ui;
    QString *usernameToAdd;
};

#endif // ADDCHATFORM_H
