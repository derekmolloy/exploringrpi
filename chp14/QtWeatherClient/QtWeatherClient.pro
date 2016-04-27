#-------------------------------------------------
#
# Project created by QtCreator 2015-11-28T23:54:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtWeatherClient
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    serversettingsdialog.cpp

HEADERS  += mainwindow.h \
    serversettingsdialog.h

FORMS    += mainwindow.ui \
    serversettingsdialog.ui
