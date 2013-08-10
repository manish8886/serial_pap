#include "ivy_dl_thread.h"
#include <Ivy/ivy.h>
#include <iostream>
void CIvyDlThread::run(){
  while(!bstop){
    char* buffer=NULL;
    int len=0;
    CTelemetryMsg *pmsg = pivymsgqueue->dequeue();
    if(pmsg==NULL)
      continue;
    len = pmsg->getBufferedMsg(&buffer);
    if(buffer==NULL)
      continue;
     IvySendMsg("%s",buffer);
    delete [] buffer;
    delete pmsg;
  }
}

