#include "telemsg.h"
#include "messages.h"
GPSMsg::GPSMsg(){
  gps_mode=0;
  gps_utm_east=0;
  gps_utm_north=0;
  gps_course=0;
  gps_alt=0;
  gps_speed=0;
  gps_climb=0;
  gps_week=0;
  gps_tow=0;
  gps_utm_zone=0;
  gps_nb_err=0;
  msgbuffer=NULL;
}
quint8 GPSMsg::msg_id = DL_GPS;
quint8 GPSMsg::msg_len = 1+4+4+2+4+2+2+2+4+1+1;

quint8 GPSMsg::getmsglength(){
  return msg_len;
}
quint8 GPSMsg::getmsgid(){
  return msg_id;
}


GPSMsg::GPSMsg(const char* buff){
  const char* temp = buff;
  gps_mode=buff[0];
  buff+=sizeof(gps_mode);

  gps_utm_east= ((quint32* )buff)[0] ;
  buff+=sizeof(quint32);

  gps_utm_north=((quint32* )buff)[0];
  buff+=sizeof(quint32);

  gps_course=((quint16*)buff)[0];
  buff+=sizeof(quint16);

  gps_alt=((quint32* )buff)[0];
  buff+=sizeof(quint32);

  gps_speed=((quint16*)buff)[0];
  buff+=sizeof(quint16);

  gps_climb=((quint16*)buff)[0];
  buff+=sizeof(quint16);


  gps_week=((quint16*)buff)[0];
  buff+=sizeof(quint16);

  gps_tow=((quint32* )buff)[0];
  buff+=sizeof(quint32);

  gps_utm_zone=((quint8* )buff)[0];
  buff+=sizeof(quint8);
  
  gps_nb_err=((quint8* )buff)[0];
  buff+=sizeof(quint8);

  //copy the buffer in the msgbuffer
  msgbuffer = new char[msg_len];
  memcpy(msgbuffer,temp,msg_len);
  
}
int GPSMsg::getBufferedMsg(char** buffer){
  if(msgbuffer==NULL){
    *buffer=NULL;
    return 0;
  }
  *buffer = new char[msg_len];
  memcpy(*buffer,msgbuffer,msg_len);
  return msg_len;
}

QString GPSMsg::getPrettyMsg(){
  QString str;
  if(msgbuffer==NULL){
    str+="msg buffer has not been initialised yet\n";
    return str;
  }
  
  str+=QString("===========================");
  str+=QString("\n");
  str+=QString("gps_mode:=");
  str+=QString::number(gps_mode);
  str+=QString("\n");


  str+=QString("gps_utm_east:=");
  str+=QString::number(gps_utm_east);
  str+=QString("\n");

  
  str+=QString("gps_utm_north:=");
  str+=QString::number(gps_utm_north);
  str+=QString("\n");


  str+=QString("gps_course:=");
  str+=QString::number(gps_course);
  str+=QString("\n");
  
  str+=QString("gps_alt:=");
  str+=QString::number(gps_alt);
  str+=QString("\n");

  str+=QString("gps_speed:=");
  str+=QString::number(gps_speed);
  str+=QString("\n");


  str+=QString("gps_climb:=");
  str+=QString::number(gps_climb);
  str+=QString("\n");


  str+=QString("gps_week:=");
  str+=QString::number(gps_week);
  str+=QString("\n");


  str+=QString("gps_tow:=");
  str+=QString::number(gps_tow);
  str+=QString("\n");

  str+=QString("gps_utm_Zone:=");
  str+=QString::number(gps_utm_zone);
  str+=QString("\n");


  str+=QString("gps_nb_err:=");
  str+=QString::number(gps_nb_err);
  str+=QString("\n");


  str+=QString("===========================");
  str+=QString("\n");

  return str;

}

quint8 IRMsg::msg_id = DL_IR_SENSORS ;
quint8 IRMsg::msg_len = 2+2+2+2+2;

IRMsg::IRMsg(){
  ir_ir1=0;
  ir_ir2=0;
  ir_longitudinal=0;
  ir_lateral=0;
  ir_vertical=0;
  msgbuffer=NULL;
}
IRMsg::IRMsg(const char* buff){
  (void)buff;
}

quint8 IRMsg::getmsglength(){
  return msg_len;
}
quint8 IRMsg::getmsgid(){
  return msg_id;
}
int IRMsg::getBufferedMsg(char** buffer){
  (void)buffer;
  return msg_len;

}
QString IRMsg::getPrettyMsg(){
  QString result;
  return result;
}
