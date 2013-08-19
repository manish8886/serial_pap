#include <QtCore/QThread>
#include <QtExtSerialPort/qextserialport.h>
#include "common.h"
#include "synchqueue.h"
class CWriterThread:public QThread{
  Q_OBJECT
    public:
  CWriterThread(QextSerialPort* pSerialport,QSynchQueue< char*>*qu):
  pqtSerialPort(pSerialport),
    pbufferedQueue(qu){
      buffer = NULL;
    }
  ~CWriterThread(){
    pbufferedQueue=NULL;
    pqtSerialPort=NULL;
  }
 protected:
  void run();
 private:
  QextSerialPort *pqtSerialPort;
  /*A shared queue from which messages need to
  be taken.*/
  QSynchQueue< char*>* pbufferedQueue;
 private:
  void writePort();
  char* buffer;
};
