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
void CMsgProcThread::processmsg(const  char* buffer){
  if(buffer==NULL)
    return;
  int i=0;
  while(i<MAX_BYTE){
    char data = buffer[i];
    if(data == CTelemetryMsg::STX){
      int msg_len = GET_MSG_LEN(buffer,i);
      if(verifychecksum(&buffer[i])){
	CTelemetryMsg* pmsg = CMsgFactory::CreateMsg(GET_MSG_ID(buffer,i),msg_len,GET_MSG_PTR(buffer,i));
	if(pmsg==NULL){
	  std::cout <<"Couldn't form the message from message id:"<<(int)(buffer[i+3])<<std::endl;
	}
	else{
	  pmsgContainer->push_back(pmsg);
	  std::cout << pmsg->getPrettyMsg().toStdString() << std::endl;
	}
      }
      i+=msg_len;
    }else{
      i++;
    }
  }
  return;
}

void CMsgProcThread::run(){
  while(bstop==false){
    char* msgbuff = pbufferedQueue->dequeue();
    processmsg(msgbuff);
    delete [] msgbuff;
  }
  return;
}

