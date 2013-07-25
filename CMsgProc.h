#include <QtCore/QThread>
#include <QtCore/QSemaphore>
#include "common.h"
class CMsgProc:public QThread{
 public:
  CMsgProc(){
    bwaiting=true;
    bexit=false;
    bzero(buffer,MAX_BYTE);
  }
  void call_back(const char* buffer);
  void exiting(){bexit=true;}
 protected:
  void run(){
    process();
  };
 private: 
  void process();
  QSemaphore sema;
  bool bwaiting,bexit;
  char buffer[MAX_BYTE];
};
