QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addchatform.cpp \
    chat.cpp \
    chatmanager.cpp \
    client.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    notificationwin.cpp

HEADERS += \
    addchatform.h \
    chat.h \
    chatmanager.h \
    client.h \
    mainwindow.h \
    message.h \
    notificationwin.h

FORMS += \
    addchatform.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = armeabi-v7a
