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
    break;
  case DL_IR_SENSORS:
    pmsg = new IRMsg(buff);
    break;
  default:
    pmsg = NULL;
    break;
  }
  return pmsg;
}

