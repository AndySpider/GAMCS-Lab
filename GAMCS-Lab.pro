#-------------------------------------------------
#
# Project created by QtCreator 2014-06-12T10:08:06
#
#-------------------------------------------------

APPNAME = GAMCS-Lab
VERSION = 0.0.1

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

TARGET = $${APPNAME}
TEMPLATE = app

CONFIG += warn_on

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
    scene.cpp \
    viewer.cpp \
    block.cpp \
    cheese.cpp \
    nail.cpp \
    mouse.cpp \
    cat.cpp \
    spirit.cpp \
    channel.cpp \
    setdialog.cpp \
    avatarspirit.cpp \
    elephant.cpp \
    bomb.cpp \
    staticspirit.cpp \
    memhandler.cpp \
    configdialog.cpp \
    configpages.cpp \
    configure.cpp \
    randdialog.cpp

HEADERS  += mainwindow.h \
    scene.h \
    viewer.h \
    block.h \
    cheese.h \
    nail.h \
    mouse.h \
    cat.h \
    spirit.h \
    channel.h \
    setdialog.h \
    avatarspirit.h \
    elephant.h \
    bomb.h \
    staticspirit.h \
    memhandler.h \
    configdialog.h \
    configpages.h \
    configure.h \
    randdialog.h

unix|macx|win32: LIBS += -lgamcs
win32: LIBS += -lsqlite3    # windows still needs sqlite3.lib?

RESOURCES += \
    GAMCS-Lab.qrc

FORMS +=

DEFINES += GLOBAL_SENSE #	\
#    SURVIVE_MODE

VERSTR = '\\"$${VERSION}\\"'
APPSTR = '\\"$${APPNAME}\\"'
DEFINES += VER=\"$${VERSTR}\" APP=\"$${APPSTR}\"

OTHER_FILES +=
