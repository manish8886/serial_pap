#ifndef JSBSIM_THREAD_H
#define JSBSIM_THREAD_H

#include <string>
#include <iostream>

#include <QtCore/QThread>
#include <QtCore/QTimer>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
///JSBSIM library related functions.
#include <FGFDMExec.h>
#include <FGJSBBase.h>
#include <models/FGPropulsion.h>
#pragma GCC diagnostic pop

#include "generated/airframe.h"
#include "generated/flight_plan.h"


#include "telemsg.h"
#include "synchqueue.h"
/*#include "downlink_transport.h"*/
#include "hitl_transport.h"


using namespace std;
using namespace JSBSim;




#ifndef JSBSIM_PERIOD
#define JSBSIM_SPEEDUP 4 ///< how many JSBSim calls per A/P control loop call?
#define JSBSIM_PERIOD (1000.0/CONTROL_FREQUENCY/JSBSIM_SPEEDUP) ///< JSBSim timestep in milliseconds
#else
#define JSBSIM_SPEEDUP ((uint8_t) (1000./CONTROL_FREQUENCY/JSBSIM_PERIOD))
#endif
#define DT (JSBSIM_PERIOD*1e-3) ///< JSBSim timestep in seconds

#define SYSTIME_PERIOD ((uint32_t)(1000. / SYS_TIME_FREQUENCY)) ///< in msec

#define RAD2DEG 57.29578
#define FT2M 0.3048

class JSBSimThread:public QThread{
  Q_OBJECT
 public:
  JSBSimThread(QSynchQueue<char*>*pdatalink_queue,QSynchQueue<CTelemetryMsg*>*pQueue);
  ~JSBSimThread(){
    if(FDMExec)
      delete FDMExec;
    if(psimtimer){
      psimtimer->stop();
      delete psimtimer;
    }
  }
 protected:
  void run();
  
  private Q_SLOTS:
  void jsbsim_periodic();
  void jsbsim_stop();
 private:
  void jsbsim_init();
  void copy_inputs_to_jsbsim();
  void copy_outputs_from_jsbsim();
  bool check_crash_jsbsim();
  void set_value( string name, double value);
  double get_value(string name) ;
  double normalize_from_pprz(int command);
  
 private:
  /* DownlinkTransport TransportChannel;*/
  HitlTransport TransportChannel;

  QSynchQueue<CTelemetryMsg*>*ptelemetry_msg_queue;
  QTimer *psimtimer;
  JSBSim::FGFDMExec* FDMExec;
  bool run_model;
  string ICName;
  string AircraftName;
  static  quint8 gps_id;
  static  quint8 ac_id;
  static  quint8 ir_id;
};


#endif
