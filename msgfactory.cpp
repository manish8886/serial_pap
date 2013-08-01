#include "messages.h"
#include "msgfactory.h"
#include "telemsg.h"
CTelemetryMsg* CMsgFactory::CreateMsg(quint8 msg_id,quint8 len,const char* buff )
{
  CTelemetryMsg* pmsg=NULL;
  (void)len;
  switch(msg_id){
  case DL_GPS:
    pmsg = new GPSMsg(buff);
  case DL_IR_SENSORS:
    pmsg = new IRMsg(buff);
  default:
    pmsg = NULL;
  }
  return pmsg;
}

