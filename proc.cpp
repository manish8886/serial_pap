#include <iostream>
#include "CMsgProc.h"
void CMsgProc::process(){
  while(bexit==false){
    sema.acquire();
    bwaiting=false;
    int index=0;
    while(index<MAX_INDEX){
      std::cout << buffer[index++];
    }
    bwaiting=true;
  }
  std::cout << std::endl;
}
void CMsgProc::call_back(const char* buffer){
  if(buffer==NULL || bwaiting==false)
    return ;
  bzero(this->buffer,MAX_INDEX);
  memcpy(this->buffer,buffer,MAX_INDEX);
  sema.release();
}
