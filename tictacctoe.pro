QT       += core gui sql
QT += sql
QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gameframe.cpp \
    gameframeai.cpp \
    gamehistory.cpp \
    main.cpp \
    mainwindow.cpp \
    secondscreen.cpp \
    tst_mainwindow.cpp

HEADERS += \
    gameframe.h \
    gameframeai.h \
    gamehistory.h \
    mainwindow.h \
    secondscreen.h

FORMS += \
    gameframe.ui \
    gameframeai.ui \
    gamehistory.ui \
    mainwindow.ui \
    secondscreen.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
