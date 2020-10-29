#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new Client(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Registration_clicked()
{
    ui->FormsAndMainMenu->setCurrentIndex(1);
}

void MainWindow::on_Logining_clicked()
{
    ui->FormsAndMainMenu->setCurrentIndex(0);
}

void MainWindow::on_Send_clicked()
{

}

void MainWindow::on_RegisterUserButton_clicked()
{
    if(ui->regPassword1->text().isEmpty() || ui->regPassword1->text().isEmpty() || ui->regUsername->text().isEmpty())
    {
        ui->regInfoLabel->setText("Smth empty.");
        QPalette palette = ui->regInfoLabel->palette();
        palette.setColor(ui->regInfoLabel->foregroundRole(), Qt::red);
        ui->regInfoLabel->setPalette(palette);
        return;
    }

    if(ui->regPassword1->text() != ui->regPassword2->text())
    {
        ui->regInfoLabel->setText("Incorect password.");
        QPalette palette = ui->regInfoLabel->palette();
        palette.setColor(ui->regInfoLabel->foregroundRole(), Qt::red);
        ui->regInfoLabel->setPalette(palette);
        return;
    }
    else
    {
        ui->regInfoLabel->setText("Trying to register you...");
        QPalette palette = ui->regInfoLabel->palette();
        palette.setColor(ui->regInfoLabel->foregroundRole(), QColor(0, 200, 0));
        ui->regInfoLabel->setPalette(palette);

        client->registerUser(ui->regUsername->text(), ui->regPassword1->text());
        connect(client, &Client::Registered, [=](bool isRegistered){
            if(isRegistered)
            {
                ui->regInfoLabel->setText("Registered successful!");
                QThread::sleep(1000);
                on_Registration_clicked();
            }
            else
            {
                QPalette palette = ui->regInfoLabel->palette();
                palette.setColor(ui->regInfoLabel->foregroundRole(), Qt::red);
                ui->regInfoLabel->setPalette(palette);
                ui->regInfoLabel->setText("Cant register you");
            }


        });
    }


}
