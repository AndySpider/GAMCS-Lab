#-------------------------------------------------
#
# Project created by QtCreator 2014-06-11T13:22:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simulated_mice
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        mainwindow.cpp \
    nail.cpp \
    cheese.cpp \
    block.cpp \
    mouse.cpp

HEADERS  += mainwindow.h \
    nail.h \
    cheese.h \
    block.h \
    mouse.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

unix:!macx|win32: LIBS += -lgamcs
