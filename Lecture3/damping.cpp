#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cfloat>
//using namespace std;                                                                                                                                       

int damp(double dt){
  char filename[128];
  std::ofstream file;
  int i=0;
  double v=10.,out=1.;
  sprintf(filename,"velo_%.4f.dat",dt);
  file.open(filename);
  while(v > DBL_EPSILON){
    v-=v*dt;
    i++;
    if((double)i >= out){
      file << (double)i*dt << "   " << v <<std::endl;
      std::cout << (double)i*dt << "   " << v <<std::endl;
      out*=1.5;
    }
  }
  file.close();
  return 0;
}


int main(){
  double dt;
  for(dt=1.e-4;dt<=1.e-1;dt*=10.)
    damp(dt);
  return 0;
}
