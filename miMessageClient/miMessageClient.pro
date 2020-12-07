QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network multimedia

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addchatform.cpp \
    callingmenu.cpp \
    chat.cpp \
    chatmanager.cpp \
    client.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    notificationwin.cpp \
    settingsmenu.cpp \
    speakingmenu.cpp

HEADERS += \
    addchatform.h \
    callingmenu.h \
    chat.h \
    chatmanager.h \
    client.h \
    mainwindow.h \
    message.h \
    notificationwin.h \
    settingsmenu.h \
    speakingmenu.h

FORMS += \
    addchatform.ui \
    callingmenu.ui \
    mainwindow.ui \
    settingsmenu.ui \
    speakingmenu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = armeabi-v7a

RESOURCES += \
    resources.qrc
