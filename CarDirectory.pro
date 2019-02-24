#-------------------------------------------------
#
# Project created by QtCreator 2019-02-19T16:09:09
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CarDirectory
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
    database.cpp \
    database.cpp

HEADERS  += mainwindow.h\
    database.h \
    database.h \
    DataBaseRow.h

FORMS    += mainwindow.ui
