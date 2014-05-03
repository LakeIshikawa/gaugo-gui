#-------------------------------------------------
#
# Project created by QtCreator 2014-05-01T22:07:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GauGoUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gaugotalker.cpp \
    boardview.cpp \
    boarditem.cpp

HEADERS  += mainwindow.h \
    gaugotalker.h \
    boardview.h \
    boarditem.h

FORMS    += mainwindow.ui
