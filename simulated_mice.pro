#-------------------------------------------------
#
# Project created by QtCreator 2014-06-11T13:22:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simulated_mice
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    spirit.cpp \
    nail.cpp \
    cheese.cpp \
    block.cpp

HEADERS  += mainwindow.h \
    spirit.h \
    nail.h \
    cheese.h \
    block.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
