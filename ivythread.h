#ifndef IVY_THREAD_H 
#define IVY_THREAD_H 
#include <QtCore/QThread>
#include "downlink_transport.h"
class IvyThread:public QThread{
  Q_OBJECT
    public:
  IvyThread(QSynchQueue< char*> *pqueue):
  TransportChannel(pqueue){
    
  }
  public Q_SLOTS:
  void stop_processing();
 protected:
  void run();
 private:
  void ivy_transport_init(void);
  void sim_autopilot_init(void);
 private:
  DownlinkTransport TransportChannel;
};

#endif
