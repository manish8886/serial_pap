#include <QtCore/QThread>
#include <QtExtSerialPort/qextserialport.h>
#include <QtCore/QQueue>
#include <QtCore/QSemaphore>
#include "CMsgProc.h"
#include "common.h"
class CReaderThread:public QThread{
 public:
 CReaderThread(QextSerialPort* pSerialport,QQueue<char*>*qu,QSemaphore* qusema):
  pqtSerialPort(pSerialport),
    pbufferedQueue(qu),
    pqueuesema(qusema)
  {

  }
  ~CReaderThread(){
    pbufferedQueue=NULL;
    pqtSerialPort=NULL;
    pqueuesema=NULL;
  }
 protected:
  void run();
 private:
  QextSerialPort *pqtSerialPort;
  QQueue<char*>* pbufferedQueue;
  QSemaphore* pqueuesema;
 private:
  void readPort();
  char buffer[MAX_BYTE];
};
