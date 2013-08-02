#include <QtCore/QThread>
#include <QtExtSerialPort/qextserialport.h>
#include <QtCore/QQueue>
#include <QtCore/QSemaphore>
#include "CMsgProc.h"
#include "common.h"
class CReaderThread:public QThread{
  Q_OBJECT
 public:
 CReaderThread(QextSerialPort* pSerialport,QQueue< char*>*qu,QSemaphore* qusema):
  pqtSerialPort(pSerialport),
    pbufferedQueue(qu),
    pqueuesema(qusema){
    }
  ~CReaderThread(){
    pbufferedQueue=NULL;
    pqtSerialPort=NULL;
    pqueuesema=NULL;
  }
 signals:
  void queuenotempty();

 protected:
  void run();
 private:
  QextSerialPort *pqtSerialPort;
  //A shared queue to put the incoming message from
  //serial port.
  QQueue< char*>* pbufferedQueue;
  /*A shared Semaphore for syncronizon for the 
    above queue*/
  QSemaphore* pqueuesema;
 private:
  void readPort();
  char buffer[MAX_BYTE];
};
