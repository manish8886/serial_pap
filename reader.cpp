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
  bzero(buffer,MAX_BYTE);
  while(pqtSerialPort->isOpen()){
    qint64 status= pqtSerialPort->read(&buffer[i++],1);
    if(status<=0){
      i--;
    }
    if(i==MAX_BYTE){
      i=0;
      if(pProcessor)
	pProcessor->call_back(buffer);
      bzero(buffer,MAX_BYTE);
    }
    
  }
  
  if(pProcessor)
    pProcessor->exiting();

  std::cout << "reader thread is now exiting"<<std::endl;
  return;
}
