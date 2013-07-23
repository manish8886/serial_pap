#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T12:25:27
#
#-------------------------------------------------

QT       += core gui

TARGET = serial_pap
TEMPLATE = app


SOURCES += main.cpp\
enumport.cpp

#HEADERS  += mainwindow.h

#FORMS    += mainwindow.ui

#unix:!macx:!symbian: LIBS += -L$$PWD/qextserialport/ -lqextserialport

#INCLUDEPATH += $$PWD/qextserialport
#DEPENDPATH += $$PWD/qextserialport

CONFIG += extserialport

#unix:!macx:!symbian: LIBS += -livy
