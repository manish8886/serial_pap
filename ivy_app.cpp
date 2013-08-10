#include <iostream>
#include <Ivy/ivy.h>
#include <Ivy/ivyloop.h>
#include "ivy_init.h"
#include "ivy_app.h"

void CIVY_APP::stop_processing(){
  IvyStop();
}
void CIVY_APP::run(){
  ivy_transport_init();
  /* main loop */
  IvyMainLoop();
  
}
