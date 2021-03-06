QT -= gui
QT += network sql

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        audioserver.cpp \
        chat.cpp \
        chatmember.cpp \
        databasecontroller.cpp \
        group.cpp \
        groupmember.cpp \
        main.cpp \
        message.cpp \
        server.cpp \
        user.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    audioserver.h \
    chat.h \
    chatmember.h \
    databasecontroller.h \
    group.h \
    groupmember.h \
    message.h \
    server.h \
    user.h
