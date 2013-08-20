#include "procthread.h"
#include <QtCore/QDebug>
#include <iostream>
#include "messages.h"
#include "msgfactory.h"
#include "common.h"

#define MSG_HEADER_LEN        ((1+1+1+1))/*STX+MSG_LEN+AC_ID|MSG_ID*/
#define MSG_TAIL_LEN          ((1+1)) /*ck_a+ck_b*/
#define MOVE_TO_DATA(buffer,start)  (buffer=&(((char*)buffer)[start+MSG_HEADER_LEN]))
#define GET_MSG_LEN(buffer,start)    ((buffer[start+1]))
#define GET_MSG_ID(buffer,start)    ((buffer[start+3]))
#define GET_MSG_PTR(buffer,start)   (&(buffer[start+MSG_HEADER_LEN]))
#define GET_IVY_PAYLOAD_PTR(buffer,start)   (&(buffer[start+IvyMsg::MARKERBYTES-1]))


/*
  Message Format:
  ----------------------------------------------------------------------------------------
  | |STX|MSG_LEN|AC_ID|MSG_ID|-----Actual Msg----|ck_a|ck_b|                               |
  | total length=len(stx)+len(MSG_LEN)+len(AC_ID)+len(MSG_ID)+len(MSG)+len(ck_a)+len(ck_b) |
  ----------------------------------------------------------------------------------------
*/
bool CMsgProcThread::verifychecksum(const char* buffer){
  quint8 checksum_a=0,checksum_b=0,ck_a=0,ck_b=0,msg_len=0;
  int i=0;
  if(buffer[0]!=CTelemetryMsg::STX ){
    qDebug()<<"Incoorect Input to verify checksum\n";
    return false;
  }
  
  msg_len = buffer[1];

  if(msg_len <MSG_TAIL_LEN)
    return false;

  i=1;/*start from the message len*/
  while(i!=(msg_len-MSG_TAIL_LEN)){
    checksum_a+=buffer[i++]; 
    checksum_b+=checksum_a;
  }
  
  ck_a = buffer[i++];
  ck_b = buffer[i++];

  if(ck_a != checksum_a){
    //   qDebug()<<"Recived data has checksum error ck_a!=checksum_a\n";
    return false;
  }
  if(checksum_b!=ck_b){
    // qDebug()<<"Recived data has checksum error ck_a!=checksum_a\n";
    return false;
  }
  return true;
}
void CMsgProcThread::processmsg( char* buffer){
  if(buffer==NULL)
    return;
  int i=0;
  while(i<MAX_BYTE){
    char start_byte = buffer[i];
    CTelemetryMsg* pmsg=NULL;
    bool isMsgValid=false;
    int msg_len=0;
    quint8 msg_id=0;
    switch(start_byte){
    case CMsg::STX:{
      isMsgValid = verifychecksum(&buffer[i]);
      if(isMsgValid){
	msg_len = GET_MSG_LEN(buffer,i);
	pmsg = CMsgFactory::CreateMsg(GET_MSG_ID(buffer,i),msg_len,GET_MSG_PTR(buffer,i));
      }
      if(pmsg){
	pmsgContainer->enqueue(pmsg);
	std::cout << pmsg->getPrettyMsg().toStdString() << std::endl;

      }else{
	std::cout <<"Couldn't form the message from message id:"<<(int)(buffer[i+3])<<std::endl;
      }

    }
      break;
    case CMsg::IVY_START:{
      isMsgValid = IvyMsg::verifyMsg(&buffer[i]);
      if(isMsgValid){
	msg_len = buffer[i+1];
	pmsg = CMsgFactory::CreateMsg(DL_IVY_MSG_ID,msg_len,&buffer[i]);
      }
      if(pmsg){
	pivymsgqueue->enqueue(pmsg);
	if(pmsg->isJSBSIMmsg()){
	  char* buff = NULL;
	  pmsg->getBufferedMsg(&buff);
	  if(buff){
	    std::cout << "COMMANDS MSG is:" << buff <<std::endl;
	    delete [] buff;
	  }
	  
	  pjsbsimqueue->enqueue(pmsg);
	}
      }
    }
      break;
    default:
      break;
      
    }
    
    isMsgValid?(i+=msg_len):(i++);
  }
  return;
}



void CMsgProcThread::run(){
  while(bstop==false){
    char* msgbuff = pbufferedQueue->dequeue();
    if(msgbuff==NULL)
      continue;
    processmsg(msgbuff);
    delete [] msgbuff;
  }
  std::cout<<"now processor thread is exiting" << std::endl;
  return;
}

