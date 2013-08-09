#include <iostream>
#include "ivy_init.h"
#include "listener.h"

extern int enumerateserialports();
extern void setserialconf(PortSettings& portsettings);
#include "common.h"
ListenApp::ListenApp(int argc, char *argv[]):
  QCoreApplication(argc,argv) {
  //QCoreApplication a(argc, argv);
  pqtSerialPort =NULL;
  ptimer = NULL;
  PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 0};
  pqtSerialPort = new QextSerialPort ("/dev/ttyS0",settings,QextSerialPort::Polling);
  
  /*create timer but don't start it now*/
  ptimer = new QTimer();
  
  //first create threads
  reader = new CReaderThread(pqtSerialPort,&msgbuffqueue);
  processor = new CMsgProcThread(&msgbuffqueue,&telemsgcontainer,&ivyqueue);
   ivy_transport_init();
  
}
ListenApp::~ListenApp(){
  if(pqtSerialPort)
    delete pqtSerialPort;
  if(ptimer)
    delete ptimer;

}
bool ListenApp::setup(int brate,int time){
  //  enumerateserialports();
  if(brate==9600){
    pqtSerialPort->setBaudRate(BAUD9600);
  }else{
    std::cout << "Baud Rate Not Supported Yet"<<std::endl;
    return 0;
  }

  QObject::connect(ptimer,SIGNAL(timeout()),this,SLOT(closeapp()));
  
  if(pqtSerialPort->open(QIODevice::ReadWrite)==false){
    std::cout << "couldn't open the serial port";
    pqtSerialPort->close();
    return 0;
  }else{
    std::cout<<"serial port opened succesfully";
  }
  //first connect to dsr signal of pqtSerialPort
  connect(pqtSerialPort,SIGNAL(dsrChanged ( bool )),
	  this,SLOT(serial_port_dsr_event(bool )));
  
  /*connect processor thread slot to serial port closed signal*/
  connect(this,SIGNAL(serial_port_closed()),processor,SLOT(stop_processing()));
  
  /*set the timer interval*/
  ptimer->setInterval(time);
  /*start timer*/
  ptimer->start();
  
  /*start reader*/
  reader->start();
  /*start processor*/
  processor->start();
 
  return true;
}
void ListenApp::closeapp(){
  std::cout<< "app is being closed";
  emit serial_port_closed();
  if(pqtSerialPort){
    pqtSerialPort->close();
  }
  if(ptimer){
    ptimer->stop();
  }
  
  /*Make every queue non blocking*/
  msgbuffqueue.makequeuenonblocking();
  ivyqueue.makequeuenonblocking();
  telemsgcontainer.makequeuenonblocking();
  /*wait for finshing of both threads*/

  if(reader)
    reader->wait();
  if(processor){
    processor->wait();
  }
  exit(0);
}

void ListenApp::serial_port_dsr_event(bool bstatus){
  if(bstatus)/*currently not using anything when connection starts*/
    return;
  qDebug()<<"serial port has been disconnected\n";
  closeapp();
}
