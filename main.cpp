#include <QtCore/QThread>
#include <iostream>
#include "listener.h"
int main(int argc, char *argv[]){
    int baudrate=9600;
    int time = 3000*60;// i minute
    if(argc>2){
      std::cout<<"Too many arguments"<<std::endl;
    }
    int index=0;
    while(index<argc){
      if(argv[index][0]!='-' || argv[index][2]!='=' ){
	std::cout << "Invalid characters in arugments"<<std::endl;
	return 0;
      }
      QString argstring(argv[index]+2); 
      bool bResult=true;
      if(argv[index][1]=='b'){
	baudrate = argstring.toInt(&bResult);
      }else if(argv[index][1]=='t'){
	time = argstring.toInt(&bResult);
      }else{
	std::cout << "Invalid command line option"<<std::endl;
      }
      if(!bResult){
	std::cout<< "Invalid values for the command line options" <<std::endl;
	return 0;
      } 
      index++;
    }
    ListenApp a(argc, argv);
    if(a.setup(baudrate,time)==false){
      return 0;
    }
      
    return a.exec();
}
