#ifndef NOTIFICATIONWIN_H
#define NOTIFICATIONWIN_H

#include <QObject>
#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QLabel>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QApplication>

class NotificationWin : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float opacity);
    float getPopupOpacity();
public:
    NotificationWin(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void setPopupText(const QString& text);
    void show();
private slots:
    void hideAnimation();
    void hide();

private:
    QLabel label;
    QGridLayout layout;
    QPropertyAnimation animation;
    float popupOpacity;
    QTimer *timer;
};

#endif // NOTIFICATIONWIN_H
