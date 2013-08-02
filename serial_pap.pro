#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T12:25:27
#
#-------------------------------------------------

QT       += core gui

TARGET = serial_pap
TEMPLATE = app


SOURCES +=  enumport.cpp \
 listener.cpp \
 main.cpp \
 msg.cpp \
 msgfactory.cpp \
proc.cpp \
reader.cpp \
procthread.cpp



#HEADERS  += mainwindow.h

#FORMS    += mainwindow.ui

#unix:!macx:!symbian: LIBS += -L$$PWD/qextserialport/ -lqextserialport

#INCLUDEPATH += $$PWD/qextserialport
#DEPENDPATH += $$PWD/qextserialport

CONFIG += extserialport

#unix:!macx:!symbian: LIBS += -livy

HEADERS += \
    CMsgProc.h \
    common.h \
    listener.h \
    messages.h \
    msgfactory.h \
    procthread.h \
    reader.h \
    telemsg.h \
    syncqueue.h

QMAKE_CXXFLAGS += -funsigned-char
    
DEFINES+=DOWNLINK
