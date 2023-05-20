#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cfloat>
#include "BM.h"

#define Np 1024
#define L 60.0
#define tmax 100
#define dt 0.01
#define temp 2.0
#define dim 2
#define cut 2.5

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
    a[i]=1.0+0.1*gaussian_rand();
}

void p_boundary(double (*x)[dim]){
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++)
      x[i][j]-=L*floor(x[i][j]/L);
}

void ini_array(double (*x)[dim]){
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++)
      x[i][j]=0.0;
}

void calc_force(double (*x)[dim],double (*f)[dim],double *a){
  double dx,dy,dr2,dUr,w2,w6,w12,aij;

  ini_array(f);
    
  for(int i=0;i<Np;i++)
    for(int j=0;j<Np;j++){
      if(i<j){
	dx=x[i][0]-x[j][0];
	dy=x[i][1]-x[j][1];
	//periodic boundary
	dx-=L*floor((dx+0.5*L)/L);
	dy-=L*floor((dy+0.5*L)/L);
	dr2=dx*dx+dy*dy;

	if(dr2<cut*cut){
	  aij=0.5*(a[i]+a[j]);
	  w2=aij/dr2;
	  w6=w2*w2*w2;
	  w12=w6*w6;
	  dUr=-48.*w12/dr2+24.*w6/dr2;
	  f[i][0]-=dUr*dx;
	  f[j][0]+=dUr*dx;
	  f[i][1]-=dUr*dy;
	  f[j][1]+=dUr*dy;
	}
      }
    }
}

void eom(double (*v)[dim],double (*x)[dim],double (*f)[dim],double temp0){
  double zeta=1.0;
  double fluc=sqrt(2.*zeta*temp0*dt);
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++){
      v[i][j]+=-zeta*v[i][j]*dt+f[i][j]*dt+fluc*gaussian_rand();
      x[i][j]+=v[i][j]*dt;
    } 
}


void output(double (*x)[dim],double *a){
  char filename[128];
  std::ofstream file;
  static int j=0; 
  sprintf(filename,"coord_T%.3f_%d.dat",temp,j);
  file.open(filename); 
  for(int i=0;i<Np;i++)
    file <<x[i][0]<<"\t"<<x[i][1]<<"\t"<<a[i]<<std::endl;
  file.close();
  j++;
}

int main(){
  double x[Np][dim],v[Np][dim],f[Np][dim],a[Np];
  double tout=0.0;
  int j=0;
  set_diameter(a);
  ini_coord_square(x);
  ini_array(v);
  
  while(j*dt < 10.0){
    j++;
    calc_force(x,f,a);
    eom(v,x,f,5.0);
  }
  j=0;
  while(j*dt < tmax){   
    j++;
    calc_force(x,f,a);
    eom(v,x,f,temp);
    p_boundary(x);
    if(j*dt >= tout){
      output(x,a);
      tout+=10.;
    }
  }
  return 0;
}
