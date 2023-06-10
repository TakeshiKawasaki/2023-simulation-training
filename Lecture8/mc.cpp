#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cfloat>
#include "BM.h"

#define Np 1024
#define L 40.0
#define mcstep_max 100000
#define temp 0.2
#define dim 2
#define cut 2.5
#define delta 0.15
#define polydispersity 0.0

void ini_coord_square(double (*x)[dim]){
  int num_x = (int)sqrt(Np)+1;
  int num_y = (int)sqrt(Np)+1;
  int i,j,k=0;
  for(j=0;j<num_y;j++){
    for(i=0;i<num_x;i++){
      x[i+num_x*j][0] = i*L/(double)num_x;
      x[i+num_x*j][1] = j*L/(double)num_y;
      k++;
      if(k>=Np)
        break;
    }
    if(k>=Np)
      break;
  }
}

void set_diameter(double *a){
  for(int i=0;i<Np;i++)
    a[i]=1.0+polydispersity*gaussian_rand();
}


void p_boundary(double (*x)[dim],int i){
  for(int j=0;j<dim;j++)
    x[i][j]-=L*floor(x[i][j]/L);
}

void ini_array(double (*x)[dim]){
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++)
      x[i][j]=0.0;
}

void calc_energy(double (*x)[dim],double *a,double *U){
  double dx,dy,dr2,w2,w6,w12,aij;
  //  double Ucut=4.*(pow(cut,-12)-pow(cut,-6));

  *U=0.;
  for(int i=0;i<Np;i++)
    for(int j=0;j<Np;j++){
      if(j<i){
	dx=x[i][0]-x[j][0];
	dy=x[i][1]-x[j][1];
	dx-=L*floor((dx+0.5*L)/L);
	dy-=L*floor((dy+0.5*L)/L);
	dr2=dx*dx+dy*dy;
	if(dr2<cut*cut){
	  aij=0.5*(a[i]+a[j]);
	  w2=aij*aij/dr2;
	  w6=w2*w2*w2;
	  w12=w6*w6;  
	  *U+=4.*(w12-w6);
	}
      }
    }
}

void ini_energy(double (*x)[dim],double *a,double *U){
  calc_energy(x,a,&(*U));
}

void mc(double (*x)[dim],double *a,double *U,double temp0,int *count){
  double dr[2];
  double U0;
  double p;
  int  i = (int)(Np*unif_rand(0,1.0));
  U0=*U;
  for(int k=0;k<dim;k++){
    dr[k]=delta*unif_rand(-1.0,1.0);
    x[i][k]+=dr[k];
  }
  calc_energy(x,a,&(*U));
  p=unif_rand(0,1.0);
  if(p > 1./exp((*U-U0)/temp0)){
    *count+=1;
    for(int k=0;k<dim;k++)
      x[i][k]-=dr[k];
    *U=U0;
  }
  p_boundary(x,i);
}

void output(double (*x)[dim],double *a){
  char filename[128];
  std::ofstream file;
  static int j=0; 
  sprintf(filename,"coord_mc_T%.3f_%d.dat",temp,j);
  file.open(filename); 
  for(int i=0;i<Np;i++)
    file <<x[i][0]<<"\t"<<x[i][1]<<"\t"<<a[i]<<std::endl;
  file.close();
  j++;
}

int main(){
  double x[Np][dim],U,a[Np];
  int j=0,out=0,count=0;
  set_diameter(a);
  ini_coord_square(x);
  ini_energy(x,a,&U);
   
  while(j < 1000*Np){
    j++;
    mc(x,a,&U,5.0,&count);
  }
  j=0;
  count=0;
  while(j < mcstep_max*Np){
    j++;
    mc(x,a,&U,temp,&count);
    if(j>out){
      output(x,a);
      out+=1000*Np;   
    }
  }
  std::cout<<"rejection rate = "<<(double)count/mcstep_max/Np<<std::endl;
  return 0;
}
