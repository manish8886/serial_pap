#include <QtCore/QThread>
#include <QtExtSerialPort/qextserialport.h>
#include "common.h"
#include "synchqueue.h"
class CReaderThread:public QThread{
  Q_OBJECT
    public:
  CReaderThread(QextSerialPort* pSerialport,QSynchQueue< char*>*qu):
  pqtSerialPort(pSerialport),
    pbufferedQueue(qu){
    }
  ~CReaderThread(){
    pbufferedQueue=NULL;
    pqtSerialPort=NULL;
  }
 protected:
  void run();
 private:
  QextSerialPort *pqtSerialPort;
  //A shared queue to put the incoming message from
  //serial port.
  QSynchQueue< char*>* pbufferedQueue;
 private:
  void readPort();
  char buffer[MAX_BYTE];
};
