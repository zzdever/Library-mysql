#-------------------------------------------------
#
# Project created by QtCreator 2014-03-12T19:08:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MySqlGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -lmysqlclient

RC_FILE += /home/dever/dev/mysql/icon.rc

OTHER_FILES += \
    icon.rc
