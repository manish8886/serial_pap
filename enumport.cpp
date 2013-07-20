/**
 * @file main.cpp
 * @brief Main file.
 * @author Micha? Policht
 */
#include <QtCore/QList>
#include <QtCore/QDebug>
#include <QtExtSerialPort/qextserialenumerator.h>
#include <QtExtSerialPort/qextserialport.h>

int enumerateserialports(){
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    qDebug() << "List of ports:";
    foreach (QextPortInfo info, ports) {
      qDebug() << "port name:"       << info.portName;
      qDebug() << "friendly name:"   << info.friendName;
      qDebug() << "physical name:"   << info.physName;
      qDebug() << "enumerator name:" << info.enumName;
      qDebug() << "vendor ID:"       << info.vendorID;
      qDebug() << "product ID:"      << info.productID;
      
      qDebug() << "===================================";
    }
    return 0;
}


void setserialconf(QextSerialPort& qtSerialPort){
  qtSerialPort.setQueryMode(QextSerialPort::Polling);
  qtSerialPort.setDataBits(DATA_8);
  qtSerialPort.setDtr(false);
  qtSerialPort.setFlowControl(FLOW_OFF);
  qtSerialPort.setParity(PAR_NONE);
  //qtSerialPort.setPortName("/dev/ttyS0");
  qtSerialPort.setStopBits(STOP_1);
  qtSerialPort.setBaudRate(BAUD38400);
}
