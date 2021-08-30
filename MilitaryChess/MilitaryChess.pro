QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    checkboard.cpp \
    chessnode.cpp \
    ipset.cpp \
    ipvalidation.cpp \
    main.cpp \
    mainwindow.cpp \
    mapnode.cpp \
    myserver.cpp \
    mysocket.cpp

HEADERS += \
    checkboard.h \
    chessnode.h \
    ipset.h \
    ipvalidation.h \
    mainwindow.h \
    mapnode.h \
    myserver.h \
    mysocket.h

FORMS += \
    ipset.ui \
    ipvalidation.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../Images/png/Images.qrc
