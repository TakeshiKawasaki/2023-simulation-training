#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cfloat>
#include "BM.h"

#define Np 1024
#define Nn 100
#define L 40.0
#define mcstep_max 100000
#define temp 0.2
#define dim 2
#define cut 2.5
#define skin 1.0
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


void list_verlet(int (*list)[Nn],double (*x)[dim]){
  double dx,dy,dr2;
  double thresh=cut+skin;
  for(int i=0;i<Np;i++)
    for(int j=0;j<Nn;j++)
      list[i][j]=0;
  
  for(int i=0;i<Np;i++)
    for(int j=0;j<Np;j++){
      if(j>i){
	dx=x[i][0]-x[j][0];
	dy=x[i][1]-x[j][1];
	dx-=L*floor((dx+0.5*L)/L);
	dy-=L*floor((dy+0.5*L)/L);	
	dr2=dx*dx+dy*dy;
	if(dr2<thresh*thresh){
	  list[i][0]++;
	  list[i][(int)list[i][0]]=j;
	}
      }
    }
}


void calc_energy(double (*x)[dim],double *a,double *U,int (*list)[Nn]){
  double dx,dy,dr2,w2,w6,w12,aij;
  //  double Ucut=4.*(pow(cut,-12)-pow(cut,-6));
  *U=0.;
  for(int i=0;i<Np;i++)
    for(int j=1;j<=list[i][0];j++){
      dx=x[i][0]-x[list[i][j]][0];
      dy=x[i][1]-x[list[i][j]][1];
      dx-=L*floor((dx+0.5*L)/L);
      dy-=L*floor((dy+0.5*L)/L);
      dr2=dx*dx+dy*dy;
      if(dr2<cut*cut){
	aij=0.5*(a[i]+a[list[i][j]]);
	w2=aij*aij/dr2;
	w6=w2*w2*w2;
	w12=w6*w6;  
	*U+=4.*(w12-w6);
      }
    }
}


void update(double (*x_update)[dim],double (*x)[dim])
{
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++)
      x_update[i][j]=x[i][j];
}


void calc_disp_max(double *disp_max,double (*x)[dim],double (*x_update)[dim],int i)
{
  double dx,dy;
  double disp;
  dx=x[i][0]-x_update[i][0];
  dy=x[i][1]-x_update[i][1];
  dx-=L*floor((dx+0.5*L)/L);
  dy-=L*floor((dy+0.5*L)/L);
  disp = dx*dx+dy*dy;
  if(disp > *disp_max)
    *disp_max =disp;
  
}

void auto_list_update(double *disp_max,double (*x)[dim],double (*x_update)[dim],int (*list)[Nn],int i){
  static int count=0;
  count++;
  calc_disp_max(&(*disp_max),x,x_update,i);
  if(*disp_max > skin*skin*0.25){
    list_verlet(list,x);
    update(x_update,x);
    *disp_max=0.0;
    //    std::cout<<"update"<<count<<std::endl;
    count=0;
  }
}


void mc(double (*x)[dim],double (*x_update)[dim],double *a,double *U,double temp0,int *count,int (*list)[Nn],double *disp_max){
  double dr[2];
  double U0;
  double p;
  int  i = (int)(Np*unif_rand(0,1.0));
  U0=*U;
  for(int k=0;k<dim;k++){
    dr[k]=delta*unif_rand(-1.0,1.0);
    x[i][k]+=dr[k];
  }
  calc_energy(x,a,&(*U),list);
  p=unif_rand(0,1.0);
  if(p > 1./exp((*U-U0)/temp0)){
    *count+=1;
    for(int k=0;k<dim;k++)
      x[i][k]-=dr[k];
    *U=U0;
  }
  p_boundary(x,i);
  auto_list_update(disp_max,x,x_update,list,i);
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
  double x[Np][dim],x_update[Np][dim],U,a[Np],disp=0.0,disp_max=0.0;
  int list[Np][Nn];
  int j=0,out=0,count=0;
  set_diameter(a);
  ini_coord_square(x);
  list_verlet(list,x);
  calc_energy(x,a,&U,list);
  
  while(j < 1000*Np){
    j++;
    mc(x,x_update,a,&U,5.0,&count,list,&disp_max);
  }
  j=0;
  count=0;
  while(j < mcstep_max*Np){
    j++;
    mc(x,x_update,a,&U,temp,&count,list,&disp_max);
    if(j>out){
      output(x,a);
      out+=1000*Np;   
    }
  }
  std::cout<<"rejection rate = "<<(double)count/mcstep_max/Np<<std::endl;
  return 0;
}
