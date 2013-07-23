#include <Ivy/ivy.h>
#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtCore/QSemaphore>
#include <QtExtSerialPort/qextserialport.h>
extern int enumerateserialports();
extern void setserialconf(PortSettings& portsettings);
#define  MAX_INDEX 256
QSemaphore sema;
  /*Buffer full*/
class CThread:public QThread{
public:
  CThread(QThread** threadArray):
    firstThread(threadArray[0]),
    secondThread(threadArray[1])
  {


  }
protected:
  void run(){
    firstThread->start();
    secondThread->start();
    sleep(5*60);
    firstThread->terminate();
    secondThread->terminate();
    sema.release();
  }

private:
  QThread* firstThread;
  QThread* secondThread;
} ;
class CMsgProc:public QThread{
public:
  CMsgProc():semaphore(1){
    semaphore.acquire();
  }
  ~CMsgProc(){
    sema.release();
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
int main(int argc, char *argv[]){
  //QCoreApplication a(argc, argv);
  (void)argc;
  (void)argv;
  enumerateserialports();
  PortSettings portSettings;
  setserialconf(portSettings);
  QextSerialPort qtSerialPort("/dev/ttyS0",portSettings,QextSerialPort::Polling);
  if(qtSerialPort.open(QIODevice::ReadWrite)){
    qDebug()<<"serial port opened succesfully";
  }else{
    qDebug() << "couldn't open the serial port";
    qtSerialPort.close();
    return 0;
  }
  CMsgProc processor;
  CPortReader reader(&qtSerialPort,&processor);
  QThread* tArray[2]={&reader,&processor};
  CThread thread(tArray);
  thread.start();
  sema.acquire();
  qtSerialPort.close();
  /*Now wait for atleast 5 minute*/
  return 0;
  //  return a.exec();
}
