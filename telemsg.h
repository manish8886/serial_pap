#ifndef CMSG_H
#define CMSG_H
#include <QtCore/QString>
#include <QtCore/QtGlobal>
class CTelemetryMsg{
 public: 
  static  const quint8 STX=0x99;
  static  const quint8 AC_ID=0x05;

 public:
  //allocate and fill buffer and return it's length.
  virtual int getBufferedMsg(char** buffer)=0;
  //Return a formatted string which can be directly
  //stramed to std output.
  virtual QString getPrettyMsg()=0;
  virtual quint8 getmsglength()=0;
  virtual quint8 getmsgid()=0;
 protected:
  char* msgbuffer;


};
class GPSMsg : public CTelemetryMsg{
 public:  
  GPSMsg();
  GPSMsg(const char* buff);

 private:
   static quint8 msg_id;
   static quint8 msg_len;

 public:
  virtual int getBufferedMsg(char** buffer);
  virtual QString getPrettyMsg();
  virtual quint8 getmsglength();
  virtual quint8 getmsgid();
 private:
  quint8  gps_mode;
  quint32 gps_utm_east;
  quint32 gps_utm_north;
  quint16 gps_course;
  quint32 gps_alt;
  quint16 gps_speed;
  quint16 gps_climb;
  qint16  gps_week;
  quint32 gps_tow;
  quint8  gps_utm_zone;
  quint8  gps_nb_err;
};
class IRMsg : public CTelemetryMsg{
 public:
  IRMsg();
  IRMsg(const char* buff);
 public:
  virtual int getBufferedMsg(char** buffer);
  virtual QString getPrettyMsg();
  virtual quint8 getmsglength();
  virtual quint8 getmsgid();
  
 public:
  static quint8 msg_id;
   static quint8 msg_len;
  
 private:
  quint16 ir_ir1;
  quint16 ir_ir2;
  quint16 ir_longitudinal;
  quint16 ir_lateral;
  quint16 ir_vertical;
};
#endif 
