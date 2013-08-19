#include "serial_port_writer.h"
#include <iostream>
#include "reader.h"
void CWriterThread::run(){
  writePort();
  return;
}
void CWriterThread::writePort(){
  if(pqtSerialPort==NULL)
    return;
  int i=0;
  qint64 status=0;
  while(pqtSerialPort->isOpen()){
    buffer = pbufferedQueue->dequeue();
    if(buffer==NULL)
      continue;
    for(i=0;i<MAX_BYTE;i++){
      status = pqtSerialPort->write(&buffer[i],1);
    }
    /*Delete and reset buffer to NULL*/
    delete [] buffer;
  }
  std::cout << std::endl;
  std::cout << "serial port is closed."<<std::endl;
  std::cout << "writer thread is now exiting"<<std::endl;
  return;
}
