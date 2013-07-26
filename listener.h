#ifndef LISTENER_H
#define LISTENER_H
#include <QtCore/QDebug>
#include <QCoreApplication>
#include <QtCore/QTimer>
#include <QtExtSerialPort/qextserialport.h>
#include "reader.h"

class ListenApp:public QCoreApplication{
    Q_OBJECT
public:
  ListenApp(int argc, char* argv[]);
  ~ListenApp();
  void setup();
public:
  QextSerialPort *pqtSerialPort;
  CMsgProc* pProc;
  CReaderThread* pReader;
  QTimer *ptimer;
  public Q_SLOTS:
  virtual void closeapp();

};

#endif // LISTENER_H