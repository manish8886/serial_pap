#include "procthread.h"
#include <QtCore/QDebug>
#include "msgfactory.h"
#include "common.h"
#define MSG_HEADER_LEN        ((1+1+1+1))/*STX+MSG_LEN+AC_ID|MSG_ID*/
#define MSG_TAIL_LEN          ((1+1)) /*ck_a+ck_b*/
#define MOVE_TO_DATA(buffer)  (buffer=&(((char*)buffer)[MSG_HEADER_LEN]))
#define GET_MSG_ID(buffer)    ((buffer[3]))
#define GET_MSG_PTR(buffer)   (&(buffer[MSG_HEADER_LEN]))
/*
  Message Format:
  ----------------------------------------------------------------------------------------
  | |STX|MSG_LEN|AC_ID|MSG_ID|-----Actual Msg----|ck_a|ck_b|                               |
  | total length=len(stx)+len(MSG_LEN)+len(AC_ID)+len(MSG_ID)+len(MSG)+len(ck_a)+len(ck_b) |
  ----------------------------------------------------------------------------------------
*/
bool CMsgProcThread::verifychecksum(const char* buffer){
  quint8 checksum=0,ck_a=0,ck_b=0,msg_len=0;
  int i=0;
  if(buffer[0]!=CTelemetryMsg::STX){
    qDebug()<<"Incoorect Input to verify checksum\n";
    return false;
  }
  msg_len = buffer[1];
  i=1;/*start from the message len*/
  while(i!=(msg_len-MSG_TAIL_LEN)){
    checksum+=buffer[i++]; 
  }
  ck_a = buffer[i++];
  ck_b = buffer[i++];

  if(ck_a != ck_b){
    qDebug()<<"Recived data has checksum error ck_a!=ck_b";
    return false;
  }
  if(checksum!=ck_a){
    qDebug()<<"calculated checksum is not equal to recived checksum";
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
      int msg_len = buffer[i+1];
      if(verifychecksum(&buffer[i])){
	CTelemetryMsg* pmsg = CMsgFactory::CreateMsg(GET_MSG_ID(buffer),msg_len,GET_MSG_PTR(buffer));
	if(pmsg==NULL)
	  qDebug() << "";
	else
	  pmsgContainer->push_back(pmsg);
      }
      i+=msg_len;
    }else{
      i++;
    }
  }
  return;
}

void CMsgProcThread::run(){
  while(!bstop){
    char* buffer = NULL;
    pqueuesema.acquire();
    pqueuesema.acquire();
  }
}
