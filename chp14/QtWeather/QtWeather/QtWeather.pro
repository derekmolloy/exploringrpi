#-------------------------------------------------
#
# Project created by QtCreator 2015-11-26T01:52:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtWeather
TEMPLATE = app
LIBS += -lwiringPi


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
