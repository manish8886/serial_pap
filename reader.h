#include <QtCore/QThread>
#include <QtExtSerialPort/qextserialport.h>
#include "CMsgProc.h"
#include "common.h"
class CReaderThread:public QThread{
 public:
 CReaderThread(CMsgProc* processor,QextSerialPort* pSerialport):
  pqtSerialPort(pSerialport),
    pProcessor(processor) {
    }
 protected:
  void run();
 private:
  void readPort();
  QextSerialPort *pqtSerialPort;
  CMsgProc* pProcessor;
  char buffer[MAX_BYTE];
};
