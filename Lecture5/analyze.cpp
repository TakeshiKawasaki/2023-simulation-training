#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cfloat>
#include "BM.h"

#define temp 0.01
#define dt 0.01
#define ensemble 10000
#define window 39
#define dim 3
//using namespace std;

void ini(double *dr2,double *corr){
  for(int i=0;i<window;i++){
    dr2[i]=0.0;
    corr[i]=0.0;
  }
}

void input(double (*x)[dim],double (*v)[dim],double *t){
  char filename[128];
  std::ifstream file;
  sprintf(filename,"coord_dt%.3fT%.3f.dat",dt,temp);
  file.open(filename);
  int asize=ensemble*window;
  for(int i=0;i<asize;i++){
    file >> t[i] >> x[i][0] >> x[i][1] >> x[i][2];
  }
  file.close();

  sprintf(filename,"vel_dt%.3fT%.3f.dat",dt,temp);
  file.open(filename);
  for(int i=0;i<asize;i++){
    file >> t[i] >> v[i][0] >> v[i][1] >> v[i][2];
    // std::cout << t[i] <<"\t"<<v[0][i]<<"\t"<<v[1][i]<<"\t"<<v[2][i]<<std::endl;
  }
  file.close();
}

void output(double *t,double *dr2,double *corr,double integral){
  char filename[128];
  std::ofstream file;
  sprintf(filename,"msd_dt%.3fT%.3f.dat",dt,temp);
  file.open(filename);
  for(int i=1;i<window;i++)
    file<<t[i]-t[0]<<"\t"<<dr2[i]<<std::endl;
  file.close();

  sprintf(filename,"corr_dt%.3fT%.3f.dat",dt,temp);
  file.open(filename);
  for(int i=1;i<window;i++)
    file<<t[i]-t[0]<<"\t"<<corr[i]<<std::endl;
  file.close();

  std::cout<<"A result of the integral ="<<integral<<std::endl;
}

void analyze(double (*x)[dim],double (*v)[dim],double *t,double *dr2,double *corr,double *integral){
  double dx[dim],corr_x[dim];
  for(int i=0;i<ensemble;i++)
    for(int j=0;j<window;j++){
      for(int k=0;k<dim;k++){
	dx[k]=(x[j+window*i][k]-x[window*i][k]);
	corr_x[k]=v[j+window*i][k]*v[window*i][k];
	dr2[j]+=(dx[k]*dx[k])/ensemble;
	corr[j]+=(corr_x[k])/ensemble;
	if(j!=window-1)
	  *integral+=0.5*(v[j+window*i][k]*v[window*i][k]+v[j+1+window*i][k]*v[window*i][k])*(t[j+1]-t[j])/ensemble;
      }
    }
}

int main(){
  double t[ensemble*window],dr2[window],corr[window],integral=0.0;
  int i,j;
  double  (*x)[dim] = new double[ensemble*window][dim];
  double  (*v)[dim] = new double[ensemble*window][dim];

  ini(dr2,corr);
  input(x,v,t);
  analyze(x,v,t,dr2,corr,&integral);
  output(t,dr2,corr,integral);
  delete[] x;
  delete[] v;
  return 0;
}
