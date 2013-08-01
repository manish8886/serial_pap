#include <QtCore/QThread>
#include <QtCore/QQueue>
#include <QtCore/QSemaphore>
#include <QtCore/QVector>
#include <QtCore/QtGlobal>
#include "telemsg.h"
class CMsgProcThread:public QThread{
 public:
 CMsgProcThread(QQueue<const  char*>*qu,QSemaphore* qsema,
		 QVector<const CTelemetryMsg*>* pmsgvec):
  pbufferedQueue(qu),
    pqueuesema(qsema),
    pmsgContainer(pmsgvec)
    {

    }
 public:
  

 private:
  void processmsg(const  char* buffer);
  /*this function always assumes that the data ponited by
    buffer always has STX as it starts byte. This function check
    for validity of the message by verfying the checksum of the buffer.
  */
  bool verifychecksum(const char* buffer);
 private:
    //A shared queue to put the incoming message from
  //serial port.
  QQueue<const  char*>* pbufferedQueue;
  /*A shared Semaphore for syncronizon for the 
    above queue*/
  QSemaphore* pqueuesema;
  QVector<const CTelemetryMsg*>* pmsgContainer;
}; 
