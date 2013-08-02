#ifndef LISTENER_H
#define LISTENER_H
#include <QtCore/QDebug>
#include <QCoreApplication>
#include <QtCore/QTimer>
#include <QtExtSerialPort/qextserialport.h>
#include <QtExtSerialPort/qextserialenumerator.h>
#include <QtCore/QSemaphore>
#include <QtCore/QVector>
#include "reader.h"
#include "procthread.h"
#include "telemsg.h"
#include "synchqueue.h"
class ListenApp:public QCoreApplication{
    Q_OBJECT
public:
  ListenApp(int argc, char* argv[]);
  ~ListenApp();
  bool setup(int brate,int time);
 private Q_SLOTS:
  void serial_port_dsr_event(bool bstatus);
 signals:
  void serial_port_closed();
private:
  CReaderThread* reader; /*reader thread.*/
  CMsgProcThread* processor;/*processor thread.*/
  QSynchQueue<char*> msgbuffqueue;
  QVector<CTelemetryMsg*>telemsgcontainer;
  QextSerialPort *pqtSerialPort;
  QTimer *ptimer;

 public Q_SLOTS:  
  void closeapp();
};

#endif // LISTENER_H
