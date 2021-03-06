#ifndef IVY_THREAD_H 
#define IVY_THREAD_H 
#include <QtCore/QThread>
#include <QtCore/QVector>
#include "downlink_transport.h"
#include "synchqueue.h"
class IvyThread:public QThread{
  Q_OBJECT
    public:
  IvyThread(QSynchQueue< char*> *pq):
  /* TransportChannel(pqueue)*/
  pqueue(pq){
  }
  ~IvyThread(){
    QVector<DownlinkTransport*>::iterator it;
    for(it=TransportChannelVector.begin();
	it!=TransportChannelVector.end();
	it++
	){
      DownlinkTransport* p = *it;
      if(p){
	delete p;
      }
    }
    
  }
  public Q_SLOTS:
  void stop_processing();
 protected:
  void run();
 private:
  void ivy_transport_init(void);
  void sim_autopilot_init(void);
 private:
  /*Make a seperate transport channel for each 
   DL message callback*/
  QVector<DownlinkTransport*>TransportChannelVector;
  /*Transmit queue for Downlink Transport*/
  QSynchQueue<char*>*pqueue;
};

#endif
