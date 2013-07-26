#include <iostream>
#include "listener.h"
extern int enumerateserialports();
extern void setserialconf(PortSettings& portsettings);
#include "common.h"
ListenApp::ListenApp(int argc, char *argv[]):
    QCoreApplication(argc,argv) {
    pqtSerialPort =NULL;
    ptimer = NULL;
 }
ListenApp::~ListenApp(){
  if(pqtSerialPort)
    delete pqtSerialPort;
  if(ptimer)
    delete ptimer;
}
/*void ListenApp::setup(){
    enumerateserialports();
    //PortSettings portSettings;
    // setserialconf(portSettings);
    PortSettings settings = {BAUD38400, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 0};
    pqtSerialPort = new QextSerialPort ("/dev/ttyS0",settings,QextSerialPort::EventDriven);
    ptimer = new QTimer();
    // ptimer->setInterval(1000);
    ptimer->start(1000);
    if(pqtSerialPort->open(QIODevice::ReadWrite)){
      qDebug()<<"serial port opened succesfully";
      QObject::connect(pqtSerialPort,SIGNAL(readyRead()),this,SLOT(recieve()));
      //QObject::connect(ptimer,SIGNAL(timeout()),this,SLOT(recieve()));
    }else{
      qDebug() << "couldn't open the serial port";
      pqtSerialPort->close();
      return ;
    }
}*/


void ListenApp::setup(){
    enumerateserialports();
    //PortSettings portSettings;
    // setserialconf(portSettings);
    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 0};
    pqtSerialPort = new QextSerialPort ("/dev/ttyS0",settings,QextSerialPort::Polling);
    ptimer = new QTimer();
    // ptimer->setInterval(1000);
    ptimer->start(1000*60);
    if(pqtSerialPort->open(QIODevice::ReadWrite)){
      qDebug()<<"serial port opened succesfully";
      pProc = new CMsgProc();
      pReader = new CReaderThread(pProc,pqtSerialPort);
      pProc->start();
      pReader->start();
      QObject::connect(ptimer,SIGNAL(timeout()),this,SLOT(closeapp()));
    }else{
      qDebug() << "couldn't open the serial port";
      pqtSerialPort->close();
      return ;
    }
}

void ListenApp::closeapp(){
  std::cout<< "app is being closed";
  if(pqtSerialPort){
    pqtSerialPort->close();
  }
  if(ptimer){
    ptimer->stop();
  }
  exit(0);
}


