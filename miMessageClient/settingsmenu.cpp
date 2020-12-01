#include "settingsmenu.h"
#include "ui_settingsmenu.h"

SettingsMenu::SettingsMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsMenu)
{
    ui->setupUi(this);

    this->setWindowTitle("Settings");

    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsMenu::save);
    this->currentStyle = ui->currentTheme->currentText();
    this->notificationEnabled = true;

    configFile = new QFile("config.cfg", parent);
    configFile->open(QFile::ReadWrite | QFile::Text);
    upload();

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

void SettingsMenu::save()
{
    configFile->resize(0);
    QJsonObject toSave;
    toSave["current-style"] = this->currentStyle;
    toSave["notifications-enabled"] = this->notificationEnabled;

    QTextStream fileStream(configFile);
    fileStream << QJsonDocument(toSave).toJson();
}

void SettingsMenu::upload()
{
    QByteArray cfgData = configFile->readAll();
    if(!cfgData.isEmpty())
    {
        QJsonDocument doc = QJsonDocument::fromJson(cfgData);
        QJsonObject saved = doc.object();

        ui->currentTheme->setCurrentText(saved["current-style"].toString());
        ui->notificationEnabled->setChecked(saved["notifications-enabled"].toBool());
    }
    else
        save();
}
