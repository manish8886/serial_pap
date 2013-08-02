#include <iostream>
#include "reader.h"
void CReaderThread::run(){
  readPort();
  return;
}
void CReaderThread::readPort(){
  if(pqtSerialPort==NULL)
    return;
  int i=0;
  char* temp = NULL;
  bzero(buffer,MAX_BYTE);
  while(pqtSerialPort->isOpen()){
    qint64 status= pqtSerialPort->read(&buffer[i++],1);
    if(status<=0){
      i--;
    }
    if(i==MAX_BYTE){
      i=0;
      temp = new char[256];
      memcpy(temp,buffer,MAX_BYTE);
      pqueuesema->acquire();
      pbufferedQueue->enqueue(temp);
      pqueuesema->release();
      bzero(buffer,MAX_BYTE);
    }
  }
  std::cout << "serial port is closed."<<std::endl;
  std::cout << "reader thread is now exiting"<<std::endl;
  return;
}
void CReaderThread::queuenotempty(){
  

}
