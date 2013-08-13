#include <iostream>
#include <string>
#include <Ivy/ivy.h>
#include <Ivy/ivyloop.h>
#include "messages.h"
#include "common.h"
#include "ivythread.h"
#include "dl_protocol2.h"


using namespace std;
#ifdef __APPLE__
string ivyBus = "224.255.255.255";
#else
string ivyBus = "127.255.255.255";
#endif
string fgAddress = "127.0.0.1";

using namespace std;


static quint64 mod_value(QString string,qint8* psign, bool* ok){
  qint64 value = string.toLongLong(ok);
  (value<0) ? (*psign = -1) : (*psign = 1);
  value*=(*psign)	   ;
  if(value>0) std::cout << "incorrect conversion of type" << std::endl;
  return value;
}


static bool fill_buffer_array(const char* const* argv,DownlinkDataType typev[],unsigned int argc,char* buffer,unsigned int bytes){
  quint32 buffer_index = 0;
  quint32 argv_index = 0;
  quint8 ob_size =0;
  qint8 sign;
  bool ok=false;
  QString argstring;
  for(;argv_index<argc;argv_index++){
    argstring = argv[argv_index];
    if(buffer_index > bytes ){
      std::cout << "buffer overflowed"<<std::endl;
    }
    switch(typev[argv_index]){
    case DL_TYPE_DOUBLE:
    case DL_TYPE_TIMESTAMP:
      echo_data_type_error("string","DL_TYPE_TIMESTAMP");
      break;
    case DL_TYPE_ARRAY_LENGTH:
    case DL_TYPE_UINT8:{
      ob_size = sizeof(quint8);
      quint8* p = (quint8*)(&buffer[buffer_index]);
      /*because these values are already unsigned we
	need not be bother about the sign of the data*/
      *p = argstring.toUInt(&ok);
      if(ok==false) echo_data_type_error("string","DL_TYPE_UINT8");
    }
      break;
    case DL_TYPE_UINT16:{
      ob_size = sizeof(quint16);
      quint16* p = (quint16*)(&buffer[buffer_index]);
      /*because these values are already unsigned and conversion
	from bigger datatype to small data type will not hurt.*/
      *p = argstring.toUInt(&ok);
      if(ok==false) echo_data_type_error("string","DL_TYPE_UINT16");
    }
      break;
    case DL_TYPE_UINT32:{
      ob_size = sizeof(quint32);
      quint32* p = (quint32*)(&buffer[buffer_index]);
      /*because these values are already unsigned and conversion
	from bigger datatype to small data type will not hurt.*/
      *p = argstring.toUInt(&ok);
      if(ok==false) echo_data_type_error("string","DL_TYPE_UINT32");
    }
      break;
    case DL_TYPE_UINT64:{
      ob_size = sizeof(qulonglong);
      qulonglong* p = (qulonglong*)(&buffer[buffer_index]);
      /*because these values are already unsigned and conversion
	from bigger datatype to small data type will not hurt.*/
      *p = argstring.toULongLong(&ok);
      if(ok==false) echo_data_type_error("string","DL_TYPE_UINT32");
    }
      break;
    case DL_TYPE_INT8:{
      ob_size = sizeof(qint8);
      qint8* p = (qint8*)(&buffer[buffer_index]);
      /*because these values are already unsigned we
	need not be bother about the sign of the data*/
      *p= (qint8)(mod_value(argstring,&sign,&ok));
      *p *=sign;
      if(ok==false) echo_data_type_error("string","DL_TYPE_INT8");
    }
      break;
    case DL_TYPE_INT16:{
      ob_size = sizeof(qint16);
      qint16* p = (qint16*)(&buffer[buffer_index]);
      /*because these values are already unsigned we
	need not be bother about the sign of the data*/
      *p=(qint16)(mod_value(argstring,&sign,&ok));
      *p *=sign;
      if(ok==false) echo_data_type_error("string","DL_TYPE_INT16");
    }
      break;
    case DL_TYPE_INT32:{
      ob_size = sizeof(qint32);
      qint32* p = (qint32*)(&buffer[buffer_index]);
      /*because these values are already unsigned we
	need not be bother about the sign of the data*/
      *p=(qint32)(mod_value(argstring,&sign,&ok));
      *p *=sign;
      if(ok==false) echo_data_type_error("string","DL_TYPE_INT32");
    }
      break;
    case DL_TYPE_INT64:{
      ob_size = sizeof(qint64);
      qint64* p = (qint64*)(&buffer[buffer_index]);
      /*because these values are already unsigned we
	need not be bother about the sign of the data*/
      *p=(qint64)(mod_value(argstring,&sign,&ok));
      *p *=sign;
      if(ok==false) echo_data_type_error("string","DL_TYPE_INT8");
    }
      break;
    case DL_TYPE_FLOAT:{
      /*Though we do have qreal type for coverting a float or 
	double value.But I am not sure aout the size of qreal. Therefore
	to be safe I am convert this value to raw float.*/
      ob_size = sizeof(float);
      float* p = (float*)(&buffer[buffer_index]);
      /*because these values are already unsigned we
	need not be bother about the sign of the data*/
      *p = argstring.toFloat(&ok);
      if(ok==false) echo_data_type_error("string","DL_TYPE_INT8");
    }
      break;
    default:
      echo_switch_nomatch_error("fill_buffer_array");
    }
    if(!ok) return false;
    buffer_index+=ob_size;
  }
  return true;
}


static void on_DL_PING(IvyClientPtr app __attribute__ ((unused)),
		       void *user_data ,
		       int argc , char *argv[]){

  int index = 0;
  QString qstring;

  DownlinkTransport *pchannel = (DownlinkTransport*)user_data;

  
  if(argc!=2){
    echo_msg_arg_error("DL_PING");
  }
  qstring = argv[index++];
  /*nbytes = qstring.toInt(&ok);
  if(ok==false){
    echo_msg_conversion_error("bytes","int");
  }*/
  qstring = argv[index++];
  if(qstring!="DL_PING"){
        echo_msg_arg_error("DL_PING");
  }
  /*DL_PING doesn't have any important number of arguments*/
  DOWNLINK_SEND_PING(pchannel);
  
}

static void on_DL_ACINFO(IvyClientPtr app __attribute__ ((unused)),
			 void *user_data ,
			 int argc , char *argv[]){
  bool ok=false;
  int index = 0,nbytes=0;
  QString qstring;
  char* buffer;

  DownlinkDataType typev[]= {
    DL_TYPE_INT16,
    DL_TYPE_INT32,
    DL_TYPE_INT32,
    DL_TYPE_INT32,
    DL_TYPE_UINT32,
    DL_TYPE_UINT16,
    DL_TYPE_INT16,
    DL_TYPE_UINT8
  };
  
  if(argc!=2){
    echo_msg_arg_error("DL_ACINFO");
  }
  qstring = argv[index++];
  nbytes = qstring.toInt(&ok);
  if(ok==false){
    echo_msg_conversion_error("bytes","int");
  }
  qstring = argv[index++];
  if(qstring!="DL_ACINFO"){
    echo_msg_arg_error("DL_ACINFO");
  }
   
  buffer = new char[nbytes];

  if(fill_buffer_array(&argv[index],typev,sizeof(typev)/sizeof(DownlinkDataType),buffer,nbytes)==false){
    std::cout<<"some error occured in while constructing the arg buffer"<<std::endl;
  }
    
    DownlinkTransport *pchannel = (DownlinkTransport*)user_data;
    
    int buffer_index=0;
    qint16 course =((quint16*)(buffer))[buffer_index];
    buffer_index+=sizeof(course);

    qint32 utm_east =((quint32*)(buffer))[buffer_index];
    buffer_index+=sizeof(utm_east);


    qint32 utm_north = ((quint32*)(buffer))[buffer_index];
    buffer_index+=sizeof(utm_north);
    
    qint32 alt = ((quint32*)(buffer))[buffer_index];
    buffer_index+=sizeof(alt);



    quint32 itow = ((quint32*)(buffer))[buffer_index];
    buffer_index+=sizeof(itow);


    quint16 speed = ((quint16*)(buffer))[buffer_index];
    buffer_index+=sizeof(speed);


    qint16 climb= ((quint16*)(buffer))[buffer_index];
    buffer_index+=sizeof(climb);


    quint8 ac_id= ((quint8*)(buffer))[buffer_index];
    buffer_index+=sizeof(ac_id);

    DOWNLINK_SEND_ACINFO(pchannel,
			 &course,
			 &utm_east,
			 &utm_north,
			 &alt,
			 &itow,
			 &speed,
			 &climb,
			 &ac_id);
    
 
      delete [] buffer;
      return;
}

static void on_DL_SETTING(IvyClientPtr app __attribute__ ((unused)),
			  void *user_data ,
			  int argc , char *argv[]){
  bool ok=false;
  int index = 0,nbytes=0;
  QString qstring;
  char* buffer;

  DownlinkDataType typev[]= {
    DL_TYPE_UINT8,
    DL_TYPE_UINT8,
    DL_TYPE_FLOAT
  };
  
  if(argc!=2){
    echo_msg_arg_error("DL_SETTING");
  }
  qstring = argv[index++];
  nbytes = qstring.toInt(&ok);
  if(ok==false){
    echo_msg_conversion_error("bytes","int");
  }
  qstring = argv[index++];
  if(qstring!="DL_SETTING"){
    echo_msg_arg_error("DL_SETTING");
  }
   
  
  buffer = new char[nbytes];

  if(fill_buffer_array(&argv[index],typev,sizeof(typev)/sizeof(DownlinkDataType),buffer,nbytes)==false){
    std::cout<<"some error occured in while constructing the arg buffer"<<std::endl;
  }
    
  DownlinkTransport *pchannel = (DownlinkTransport*)user_data;
    
  int buffer_index=0;
  quint8 _index = ((quint8*)(buffer))[buffer_index];
  buffer_index+=sizeof(_index);
  
  quint8 ac_id = ((quint8*)(buffer))[buffer_index];
  buffer_index+=sizeof(ac_id);

  
  float value = ((float*)(buffer))[buffer_index];
  buffer_index+=sizeof(value);

  DOWNLINK_SEND_SETTING(pchannel, 
			&_index,
			&ac_id,
			&value);
  
  
  delete [] buffer;


}

static void on_DL_GET_SETTING(IvyClientPtr app __attribute__ ((unused)),
			      void *user_data __attribute__ ((unused)),
			      int argc __attribute__ ((unused)), char *argv[]){


  bool ok=false;
  int index = 0,nbytes=0;
  QString qstring;
  char* buffer;

  DownlinkDataType typev[]= {
    DL_TYPE_UINT8,
    DL_TYPE_UINT8,
  };
  
  if(argc!=2){
    echo_msg_arg_error("DL_GET_SETTING");
  }
  qstring = argv[index++];
  nbytes = qstring.toInt(&ok);
  if(ok==false){
    echo_msg_conversion_error("bytes","int");
  }
  qstring = argv[index++];
  if(qstring!="GET_DL_SETTING"){
    echo_msg_arg_error("DL_GET_SETTING");
  }
   
  
  buffer = new char[nbytes];

  if(fill_buffer_array(&argv[index],typev,sizeof(typev)/sizeof(DownlinkDataType),buffer,nbytes)==false){
    std::cout<<"some error occured in while constructing the arg buffer"<<std::endl;
  }
    
  DownlinkTransport *pchannel = (DownlinkTransport*)user_data;
    
  int buffer_index=0;
  quint8 _index = ((quint8*)(buffer))[buffer_index];
  buffer_index+=sizeof(_index);
  
  quint8 ac_id = ((quint8*)(buffer))[buffer_index];
  buffer_index+=sizeof(ac_id);

  DOWNLINK_SEND_GET_SETTING(pchannel, 
			&_index,
			&ac_id);
  
  
  delete [] buffer;

}

static void on_DL_BLOCK(IvyClientPtr app __attribute__ ((unused)),
			void *user_data,
			int argc, char *argv[]){
  bool ok=false;
  int index = 0,nbytes=0;
  QString qstring;
  char* buffer;

  DownlinkDataType typev[]= {
    DL_TYPE_UINT8,
    DL_TYPE_UINT8,
  };
  
  if(argc!=2){
    echo_msg_arg_error("DL_BLOCK");
  }
  qstring = argv[index++];
  nbytes = qstring.toInt(&ok);
  if(ok==false){
    echo_msg_conversion_error("bytes","int");
  }
  qstring = argv[index++];
  if(qstring!="BLOCK"){
    echo_msg_arg_error("DL_BLOCK");
  }
   
  
  buffer = new char[nbytes];

  if(fill_buffer_array(&argv[index],typev,sizeof(typev)/sizeof(DownlinkDataType),buffer,nbytes)==false){
    std::cout<<"some error occured in while constructing the arg buffer"<<std::endl;
  }
    
  DownlinkTransport *pchannel = (DownlinkTransport*)user_data;
    
  int buffer_index=0;
  quint8 block_id = ((quint8*)(buffer))[buffer_index];
  buffer_index+=sizeof(block_id);
  
  quint8 ac_id = ((quint8*)(buffer))[buffer_index];
  buffer_index+=sizeof(ac_id);

  DOWNLINK_SEND_BLOCK(pchannel, 
		      &block_id,
		      &ac_id);
  
  
  delete [] buffer;


}

static void on_DL_MOVE_WP(IvyClientPtr app __attribute__ ((unused)),
			  void *user_data __attribute__ ((unused)),
			  int argc __attribute__ ((unused)), char *argv[]){

  
  bool ok=false;
  int index = 0,nbytes=0;
  QString qstring;
  char* buffer;

  DownlinkDataType typev[]= {
    DL_TYPE_UINT8,
    DL_TYPE_UINT8,
    DL_TYPE_INT32,
    DL_TYPE_INT32,
    DL_TYPE_INT32,
  };
  
  if(argc!=2){
    echo_msg_arg_error("DL_MOVE_WP");
  }
  qstring = argv[index++];
  nbytes = qstring.toInt(&ok);
  if(ok==false){
    echo_msg_conversion_error("bytes","int");
  }
  qstring = argv[index++];
  if(qstring!="MOVE_WP"){
    echo_msg_arg_error("MOVE_WP");
  }
   
  
  buffer = new char[nbytes];

  if(fill_buffer_array(&argv[index],typev,sizeof(typev)/sizeof(DownlinkDataType),buffer,nbytes)==false){
    std::cout<<"some error occured in while constructing the arg buffer"<<std::endl;
  }
    
  DownlinkTransport *pchannel = (DownlinkTransport*)user_data;
    
  int buffer_index=0;
  quint8 wp_id = ((quint8*)(buffer))[buffer_index];
  buffer_index+=sizeof(wp_id);
  
  quint8 ac_id = ((quint8*)(buffer))[buffer_index];
  buffer_index+=sizeof(ac_id);

  qint32 lat = ((qint32*)(buffer))[buffer_index];
  buffer_index+=sizeof(lat);

  qint32 lon = ((qint32*)(buffer))[buffer_index];
  buffer_index+=sizeof(lon);


  qint32 alt = ((qint32*)(buffer))[buffer_index];
  buffer_index+=sizeof(alt);


  DOWNLINK_SEND_MOVE_WP(pchannel, 
			&wp_id,
			&ac_id,
			&lat,
			&lon,
			&alt);
  delete [] buffer;

}
 



void IvyThread::ivy_transport_init(void) {
  IvyInit ("Paparazzi jsbsim " + AC_ID, "READY", NULL, NULL, NULL, NULL);
  IvyStart(ivyBus.c_str());
}

void IvyThread::sim_autopilot_init(void){
  void *ptr = &TransportChannel;
  IvyBindMsg(on_DL_PING, ptr, "^(\\S*) DL_PING");
  IvyBindMsg(on_DL_ACINFO, ptr, "^(\\S*) DL_ACINFO (\\S*) (\\S*) (\\S* (\\S*) (\\S*) (\\S*)) (\\S*) (\\S*)");
  IvyBindMsg(on_DL_SETTING,ptr, "^(\\S*) DL_SETTING (\\S*) (\\S*) (\\S*)");
  IvyBindMsg(on_DL_GET_SETTING,ptr, "^(\\S*) GET_DL_SETTING (\\S*) (\\S*)");
  IvyBindMsg(on_DL_BLOCK, ptr, "^(\\S*) BLOCK (\\S*) (\\S*)");
  IvyBindMsg(on_DL_MOVE_WP,ptr, "^(\\S*) MOVE_WP (\\S*) (\\S*) (\\S*) (\\S*) (\\S*)");
}

void IvyThread::stop_processing(){
  IvyStop();
}

void IvyThread::run(){
  ivy_transport_init();
  sim_autopilot_init();
  /* main loop */
  IvyMainLoop();
}
