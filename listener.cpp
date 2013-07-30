#include <iostream>
#include "listener.h"
extern int enumerateserialports();
extern void setserialconf(PortSettings& portsettings);
#include "common.h"
ListenApp::ListenApp(int argc, char *argv[]):
    QCoreApplication(argc,argv) {
    //QCoreApplication a(argc, argv);
    pqtSerialPort =NULL;
    ptimer = NULL;
}
ListenApp::~ListenApp(){
  if(pqtSerialPort)
    delete pqtSerialPort;
  if(ptimer)
    delete ptimer;
}
bool ListenApp::setup(int brate,int time){
    enumerateserialports();
    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 0};
    if(brate==9600){
      settings.BaudRate=BAUD9600; 
    }else{
      std::cout << "Baud Rate Not Supported Yet"<<std::endl;
      return 0;
    }
    pqtSerialPort = new QextSerialPort ("/dev/ttyS0",settings,QextSerialPort::Polling);
    if(pqtSerialPort->open(QIODevice::ReadWrite)){
      qDebug()<<"serial port opened succesfully";
      QObject::connect(ptimer,SIGNAL(timeout()),this,SLOT(closeapp()));
    }else{
      qDebug() << "couldn't open the serial port";
      pqtSerialPort->close();
      return 0;
    }
    //create the timer.
    ptimer = new QTimer();
    ptimer->setInterval(time);
    ptimer->start();
    return true;
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


