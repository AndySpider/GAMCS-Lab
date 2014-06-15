#-------------------------------------------------
#
# Project created by QtCreator 2014-06-12T10:08:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Simulated_Mice
TEMPLATE = app

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
    spirit.cpp

HEADERS  += mainwindow.h \
    scene.h \
    viewer.h \
    block.h \
    cheese.h \
    nail.h \
    mouse.h \
    config.h \
    cat.h \
    spirit.h

unix:!macx|win32: LIBS += -lgamcs

RESOURCES += \
    images.qrc

FORMS +=
