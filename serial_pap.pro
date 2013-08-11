#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T12:25:27
#
#-------------------------------------------------

QT       += core gui

TARGET = serial_pap
TEMPLATE = app


SOURCES +=     enumport.cpp \
    ivy_dl_thread.cpp \
    listener.cpp \
    main.cpp \
    msg.cpp \
   msgfactory.cpp \
   reader.cpp \
   procthread.cpp \
   ivy_init.cpp \
   ivy_app.cpp \ 
  dl_transport.cpp


#HEADERS  += mainwindow.h

#FORMS    += mainwindow.ui

#unix:!macx:!symbian: LIBS += -L$$PWD/qextserialport/ -lqextserialport

#INCLUDEPATH += $$PWD/qextserialport
#DEPENDPATH += $$PWD/qextserialport

CONFIG += extserialport

unix:!macx:!symbian: LIBS += -livy

HEADERS += \
    common.h \
    ivy_dl_thread.h \
    ivy_transport.h \
    listener.h \
    messages.h \
    msgfactory.h \
    procthread.h \
    reader.h \
    telemsg.h \
    synchqueue.h \
    ivy_app.h \
    downlink_transport.h

QMAKE_CXXFLAGS += -funsigned-char 
QMAKE_CXXFLAGS += -g3
    
DEFINES+=DOWNLINK
DEFINES += QT_NO_DEBUG
DEFINES += QT_NO_DEBUG_OUTPUT

