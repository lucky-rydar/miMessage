#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
