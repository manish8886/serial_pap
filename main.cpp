#include <QtCore/QThread>
#include "listener.h"
int main(int argc, char *argv[]){
  //QCoreApplication a(argc, argv);
  (void)argc;
  (void)argv;
  ListenApp a(argc, argv);
  a.setup();
  return a.exec();
}
