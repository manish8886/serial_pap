#ifndef IVY_APP_H 
#define IVY_APP_H 
#include <QtCore/QThread>

class CIVY_APP:public QThread{
  Q_OBJECT
  public Q_SLOTS:
  void stop_processing();
 protected:
  void run();
};

#endif
