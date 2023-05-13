#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cfloat>
#include "BM.h"

#define tmax 10
#define dt 0.01
#define temp 1.0
#define ensemble 10000
#define dim 3
//using namespace std;

void ini_phase(double *x,double *v){
  int i;
  for(i=0;i<dim;i++){
    x[i]=0.;
    v[i]=0.;
  }
}

void ini_clock(int *j,double *tout){
  *j=0;
  *tout=1.e-2;
}

void eom(double *v,double *x){
  int i;
  for(i=0;i<dim;i++){
    v[i]+=-v[i]*dt+sqrt(2.*temp*dt)*gaussian_rand();
    x[i]+=v[i]*dt;
  }
}

void output(double *x,double *v,int j){
  char filename[128];
  std::ofstream file;

  sprintf(filename,"coord_dt%.3fT%.3f.dat",dt,temp);
  file.open(filename,std::ios::app); //append
  file <<j*dt<<"\t"<<x[0]<<"\t"<<x[1]<<"\t"<<x[2]<<std::endl;
  //  std::cout<<j*dt<<"\t"<<x[0]<<"\t"<<x[1]<<"\t"<<x[2]<<std::endl;
  file.close();

  sprintf(filename,"vel_dt%.3fT%.3f.dat",dt,temp);
  file.open(filename,std::ios::app); //append
  file <<j*dt<<"\t"<<v[0]<<"\t"<<v[1]<<"\t"<<v[2]<<std::endl;
  file.close();
  
}

int main(){
  double x[dim],v[dim],t,tout;
  int i,j;
  ini_phase(x,v);
  for(i=0;i<ensemble;i++){
    ini_clock(&j,&tout);
    output(x,v,j);
    while(j*dt < tmax){
      j++;
      eom(v,x);
      if(j*dt >= tout){
	output(x,v,j);
	tout*=1.2;
      }
    }
  }
  return 0;
}
