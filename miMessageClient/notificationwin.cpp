#include "notificationwin.h"

NotificationWin::NotificationWin(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                       Qt::Tool |
                       Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("popupOpacity");
    connect(&animation, &QAbstractAnimation::finished, this, &NotificationWin::hide);

    label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // Устанавливаем по центру
    label.setStyleSheet("QLabel { color : white; "
                        "margin-top: 6px;"
                        "margin-bottom: 6px;"
                        "margin-left: 10px;"
                        "margin-right: 10px; }");

    layout.addWidget(&label, 0, 0);
    setLayout(&layout);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &NotificationWin::hideAnimation);
}

void NotificationWin::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    painter.setBrush(QBrush(QColor(0,0,0,180)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}

void NotificationWin::setPopupText(const QString &text)
{
    label.setText(text);
    adjustSize();
}

void NotificationWin::show()
{
    setWindowOpacity(0.0);

    animation.setDuration(150);
    animation.setStartValue(0.0);
    animation.setEndValue(1.0);

    setGeometry(QApplication::screens()[0]->availableGeometry().width() - 36 - width() + QApplication::screens()[0]->availableGeometry().x(),
                QApplication::screens()[0]->availableGeometry().height() - 36 - height() + QApplication::screens()[0]->availableGeometry().y(),
                width(),
                height());

    QWidget::show();

    animation.start();
    timer->start(3000);
}

void NotificationWin::hideAnimation()
{
    timer->stop();
    animation.setDuration(1000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
}

void NotificationWin::hide()
{
    if(getPopupOpacity() == 0.0){
        QWidget::hide();
    }
}

void NotificationWin::setPopupOpacity(float opacity)
{
    popupOpacity = opacity;

    setWindowOpacity(opacity);
}

float NotificationWin::getPopupOpacity()
{
    return popupOpacity;
}
