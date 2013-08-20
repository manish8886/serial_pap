#include <QtCore/QtGlobal>
#include <QtCore/QStringList>
#include "jsbsim_thread.h"
#include "paparazzi.h"
#include  "dl_protocol2.h"
#include "messages.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <math/FGLocation.h>
#pragma GCC diagnostic pop


quint8 JSBSimThread::gps_id=1;
quint8 JSBSimThread::ac_id=AC_ID;
quint8 JSBSimThread::ir_id=1;

JSBSimThread::JSBSimThread(QSynchQueue<char*>*pdatalink_queue,QSynchQueue<CTelemetryMsg*>*pQueue)
  :TransportChannel(pdatalink_queue),
   ptelemetry_msg_queue(pQueue){
  FDMExec = new JSBSim::FGFDMExec();
  SimTimer.setInterval(JSBSIM_PERIOD);
   }

void JSBSimThread::set_value(string name, double value) {
  FDMExec->GetPropertyManager()->GetNode(name)->setDoubleValue(value);
}


void JSBSimThread::jsbsim_periodic(){
  bool result;
  /* read actuators positions and feed JSBSim inputs */
  copy_inputs_to_jsbsim();
  if (run_model) {
    result = FDMExec->Run();
  }
  /* check if still flying */
  result =  check_crash_jsbsim();
  if(!result){
    std::cout << "plane has crashed therefore exiting" << std::endl;
    exit(0);
  }
  
  /* read outputs from model state */
  copy_outputs_from_jsbsim();
}

double JSBSimThread::normalize_from_pprz(int command){
  double cmd_norm = (double)command / MAX_PPRZ;
  BoundAbs(cmd_norm, MAX_PPRZ);
  return cmd_norm;
}
void JSBSimThread::copy_inputs_to_jsbsim(){
  
  static double throttle_slewed = 0.;
  static double th = 0.;
#ifndef JSBSIM_LAUNCHSPEED
#define JSBSIM_LAUNCHSPEED 20.0 //launch speed in m/s aligned with airframe body forward
#endif
  if (run_model) th += 0.01;
  if (th >= 1) th = 1;
  // detect launch
  if (!run_model ) {
    run_model = true;
    FDMExec->GetIC()->SetUBodyFpsIC( JSBSIM_LAUNCHSPEED / FT2M);
    FDMExec->RunIC();
    th = 0.;
  }
  CTelemetryMsg* pmsg = ptelemetry_msg_queue->dequeue();
  if(pmsg==NULL){
    return;
  }
  
  qint16 *commands = new qint16[COMMANDS_NB];
  char* payload=NULL;
  pmsg->getBufferedMsg(&payload);
  QString qpayload(payload);
  QStringList strList =qpayload.split(" ",QString::SkipEmptyParts);
  
  quint8 ac_id;
  QString msg_name;
  QString qcommands_array;
  
  if(qpayload.size()<3){
    std::cout << "Incomplete JSBSIM Msg Received " << std::endl;
  }
    
  ac_id = strList[0].toUInt();
  msg_name = strList[1];
  if(msg_name != "COMMANDS"){
    std::cout << "Incorrect JSBSIM Msg Received " << std::endl;
  }
  
  qcommands_array = strList[2];
  
  QStringList commands_value = qcommands_array.split(",",QString::SkipEmptyParts);

  if(commands_value.size()!=COMMANDS_NB){
    std::cout << "Incomplete Commands array Received " << std::endl;
  }
  
  for(int i=0; i<COMMANDS_NB;i++){
    commands[i] = commands_value[i].toUInt();
  }

  double diff_throttle = normalize_from_pprz(commands[COMMAND_THROTTLE]) - throttle_slewed;
  BoundAbs(diff_throttle, 0.01);
  throttle_slewed += diff_throttle;

  set_value( "fcs/throttle-cmd-norm", throttle_slewed);
  set_value( "fcs/aileron-cmd-norm",  -normalize_from_pprz(commands[COMMAND_ROLL]));
  set_value("fcs/elevator-cmd-norm", -normalize_from_pprz(commands[COMMAND_PITCH]));
#ifdef COMMAND_YAW
  set_value( "fcs/rudder-cmd-norm",   normalize_from_pprz(commands[COMMAND_YAW]));
#endif
  
  /*Delete the buffer now.*/
  if(payload)
    delete [] payload;
  if(pmsg)
    delete pmsg;

}
double JSBSimThread::get_value(string name) {
  return FDMExec->GetPropertyManager()->GetNode(name)->getDoubleValue();
}

#define GPS_PERIOD (1./4.)

void JSBSimThread::copy_outputs_from_jsbsim(){
  
  static double gps_period = 0.;

  // copy GPS pos
  gps_period += DT;
  if (gps_period > GPS_PERIOD) {
    double lat    = get_value("position/lat-gc-rad");
    double lon    = get_value("position/long-gc-rad");
    double alt    = get_value("position/h-sl-meters");
    double course = get_value("attitude/heading-true-rad");
    double gspeed = get_value("velocities/vg-fps") * FT2M;
    double climb  = get_value("velocities/v-down-fps") * (-FT2M);
    double time   = get_value("simulation/sim-time-sec");
    

    DOWNLINK_SEND_HITL_GPS_COMMON(&TransportChannel,
				  &gps_id,
				  &ac_id,
				  &lat,
				  &lon,
				  &alt,
				  &course,
				  &gspeed,
				  &climb,
				  &time);

    gps_period = 0.;
  }
  //print(FDMExec);
  double roll   = get_value("attitude/roll-rad");
  double pitch  = get_value("attitude/pitch-rad");
  double yaw    = get_value("attitude/heading-true-rad");
  double p      = get_value("velocities/p-rad_sec");
  double q      = get_value("velocities/q-rad_sec");
  double r      = get_value("velocities/r-rad_sec");


  DOWNLINK_SEND_HITL_IR_AHRS(&TransportChannel,
			     &ir_id,
			     &ac_id,
			     &roll,
			     &pitch,
			     &yaw,
			     &p,
			     &q,
			     &r);



}
void JSBSimThread::jsbsim_stop(){
  SimTimer.stop();
  exit(0);
}

void JSBSimThread::jsbsim_init(){
  // *** SET UP JSBSIM *** //
  QByteArray root  = qgetenv("PAPARAZZI_HOME");
  if (root.data()==NULL) {
    cerr << "PAPARAZZI_HOME is not defined" << endl;
    exit(0);
  }
  string pprzRoot = string(root);
    
#ifdef JSBSIM_MODEL
  AircraftName = JSBSIM_MODEL;
#endif
#ifdef JSBSIM_INIT
  ICName = JSBSIM_INIT;
#endif
  
  FDMExec = new JSBSim::FGFDMExec();
  
  /* Set simulation time step */
  FDMExec->Setsim_time(0.);
  FDMExec->Setdt(DT);
  cout << "Simulation delta " << FDMExec->GetDeltaT() << endl;

  FDMExec->DisableOutput();
  FDMExec->SetDebugLevel(0); // No DEBUG messages

  if (!AircraftName.empty()) {

    if ( ! FDMExec->LoadModel( pprzRoot + "/conf/simulator",
                               pprzRoot + "/conf/simulator",
                               pprzRoot + "/conf/simulator",
                               AircraftName)){
      cerr << "  JSBSim could not be started" << endl << endl;
      delete FDMExec;
      exit(-1);
    }
    
    JSBSim::FGInitialCondition *IC = FDMExec->GetIC();
    if(!ICName.empty()) {
      if (!IC->Load(ICName)) {
        delete FDMExec;
        cerr << "Initialization from file unsuccessful" << endl;
        exit(-1);
      }
    }
    else {

      // FGInitialCondition::SetAltitudeASLFtIC
      // requires this function to be called
      // before itself
      IC->SetVgroundFpsIC(0.);

      // Use flight plan initial conditions
      IC->SetLatitudeDegIC(NAV_LAT0 / 1e7);
      IC->SetLongitudeDegIC(NAV_LON0 / 1e7);

      IC->SetAltitudeASLFtIC((GROUND_ALT + 2.0) / FT2M);
      IC->SetTerrainElevationFtIC(GROUND_ALT / FT2M);
      IC->SetPsiDegIC(QFU);
      IC->SetVgroundFpsIC(0.);

      //initRunning for all engines
      FDMExec->GetPropulsion()->InitRunning(-1);
      if (!FDMExec->RunIC()) {
        cerr << "Initialization from flight plan unsuccessful" << endl;
        exit(-1);
      }
    }
  } else {
    cerr << "  No Aircraft given" << endl << endl;
    delete FDMExec;
    exit(-1);
  }
}




void JSBSimThread::run(){
  jsbsim_init();
  connect(&SimTimer,SIGNAL(timeout()),this,SLOT(jsbsim_periodic()));
  SimTimer.start();
  exec();
 }

bool JSBSimThread::check_crash_jsbsim(){
  
  double agl = FDMExec->GetPropagate()->GetDistanceAGL(), // in ft
  lat = FDMExec->GetPropagate()->GetLatitude(), // in rad
  lon = FDMExec->GetPropagate()->GetLongitude(); // in rad

  if (agl< -1e-5) {
    cerr << "Crash detected: agl < 0" << endl << endl;
    return false;
  }
  if (agl > 1e5 || abs(lat) > M_PI_2 || abs(lon) > M_PI) {
    cerr << "Simulation divergence: Lat=" << lat
         << " rad, lon=" << lon << " rad, agl=" << agl << " ft" << endl
         << endl;
    return false;
  }

  if (isnan(agl) || isnan(lat) || isnan(lon)) {
    cerr << "JSBSim is producing NaNs. Exiting." << endl << endl;
    return false;
  }
  return true;
  
}
