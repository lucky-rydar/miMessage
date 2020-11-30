#include "settingsmenu.h"
#include "ui_settingsmenu.h"

SettingsMenu::SettingsMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsMenu)
{
    ui->setupUi(this);
    this->currentStyle = ui->currentTheme->currentText();

    connect(ui->currentTheme, &QComboBox::currentTextChanged, [=](QString currentText){
        this->currentStyle = currentText;
        emit this->styleChanged(currentText);
    });

    connect(ui->notificationEnabled, &QCheckBox::stateChanged, [=](int state){
        this->notificationEnabled = state;
    });

    connect(this, &SettingsMenu::styleChanged, [=](QString styleName){
        QString path = ":/styles/" + styleName + ".qss";
        QFile styleSheet(path);
        styleSheet.open(QFile::ReadOnly);
        this->setStyleSheet(styleSheet.readAll());
    });
}

SettingsMenu::~SettingsMenu()
{
    delete ui;
}

void SettingsMenu::callDefaultStyle()
{
    emit this->styleChanged(currentStyle);
}
