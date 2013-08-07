#include <QtCore/QThread>
#include <QtCore/QVector>
#include <QtCore/QtGlobal>
#include "telemsg.h"
#include "synchqueue.h"
class CMsgProcThread:public QThread{
  Q_OBJECT
 public:
  CMsgProcThread(QSynchQueue<char*>*qu, QSynchQueue<CTelemetryMsg*>* pmsgvec,
		 QSynchQueue<CTelemetryMsg*>* pivyqueue):
  pbufferedQueue(qu),
    pmsgContainer(pmsgvec),
    pivymsgqueue(pivyqueue)
    {
      bstop=false;
    }
  
  public Q_SLOTS:
  void stop_processing(){
    bstop = true;
  }
 protected:
  void run();
 private:
  bool bstop;
  void processmsg( char* buffer);
  /*this function always assumes that the data ponited by
    buffer always has STX as it starts byte. This function check
    for validity of the message by verfying the checksum of the buffer.
  */
  bool verifychecksum(const char* buffer);
 private:
  //A shared queue to put the incoming message from
  //serial port.
  QSynchQueue< char*>* pbufferedQueue;
  QSynchQueue< CTelemetryMsg*>* pmsgContainer;
  QSynchQueue< CTelemetryMsg*>* pivymsgqueue;
}; 




