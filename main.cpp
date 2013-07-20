#include <QtGui/QApplication>
#include <Ivy/ivy.h>
#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtCore/QSemaphore>
#include <QtExtSerialPort/qextserialenumerator.h>
#include <QtExtSerialPort/qextserialport.h>
extern int enumerateserialports();
extern void setserialconf(QextSerialPort& qtSerialPort);
#define  MAX_INDEX 256
  /*Buffer full*/
class CMsgProc:public QThread{
public:
  CMsgProc():semaphore(1){
    semaphore.acquire();
  }
  ~CMsgProc(){
    semaphore.release();
  }
  
protected:
  void run();
public:
  void report(char* buffer);
private:
  QSemaphore semaphore;
  char buffer[256];
};

class CPortReader:public QThread{
public:
  CPortReader(QextSerialPort* pserial,CMsgProc* preader):
    pprocessor(preader),
    pSerialport(pserial){
    buffer = new char [MAX_INDEX];
  }
  ~CPortReader(){
    delete [] buffer;
  }
protected:
  void run();
private:
  CMsgProc* pprocessor;
  QextSerialPort* pSerialport;
  char* buffer; 
  //  const int MAX_INDEX;
};
int main(int argc, char *argv[]){
  //  QCoreApplication a(argc, argv);
  (void)argc;
  (void)argv;
  enumerateserialports();
  QextSerialPort qtSerialPort;
  setserialconf(qtSerialPort);
  if(qtSerialPort.open(QIODevice::ReadWrite)){
      qDebug()<<"serial port opened succesfully";
  }else{
    qDebug() << "couldn't open the serial port";
    exit(0);
  }
  CMsgProc processor;
  CPortReader reader(&qtSerialPort,&processor);
  processor.start();
  reader.start();
  
  /*Now wait for atleast 5 minute*/
  sleep(5*60);
  reader.terminate();
  processor.terminate();
  return 0;
  //  return a.exec();
}


void CPortReader::run(){
  if(pprocessor ==NULL || pSerialport==NULL){
    qDebug() << "some parameters all Null";
    exit();
  }
  int cur_index = 0;
  while(pSerialport->isOpen()){
    /*blocking reading*/
    qint64 qstatus = pSerialport->read(&buffer[cur_index++],1);
    if(qstatus<=0){
      qDebug()<<"Error in Reading from Port";
    }
    if((cur_index==MAX_INDEX)){
      cur_index=0;/*Set it from start*/
      /*report to registered entity*/
      pprocessor->report(buffer);
    }
  }
  exit();
}
void CMsgProc::report(char* buffer){
  /*Try to obtain lock, if the other thread is busy in processing
    just return from here.*/
  if(buffer==NULL || semaphore.available()==1)
    return;
  memcpy(this->buffer,buffer,MAX_INDEX);
  semaphore.release();
  return;
}

void CMsgProc::run(){
  while(1){
    semaphore.acquire();
    qDebug()<< buffer;
  }
  exit();
}
