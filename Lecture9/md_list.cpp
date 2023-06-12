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
#define teq 100
#define tmax 30
#define dtmd 0.001
#define dtbd 0.01
#define temp 0.9
#define dim 2
#define cut 3.0
#define skin 1.5
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


void calc_force(double (*x)[dim],double (*f)[dim],double *a,double *U,int (*list)[Nn]){
  double dx,dy,dr2,dUr,w2,w6,w12,aij;
  double Ucut=1./pow(cut,12);
  ini_array(f);
  *U=0;
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
	dUr=-12.*w12/dr2;
	f[i][0]-=dUr*dx;
	f[list[i][j]][0]+=dUr*dx;
	f[i][1]-=dUr*dy;
	f[list[i][j]][1]+=dUr*dy;
	*U+=w12-Ucut;	
      }
    }
}

void eom_langevin(double (*v)[dim],double (*x)[dim],double (*f)[dim],double *a,double *U,double dt,double temp0,int (*list)[Nn]){
  double zeta=1.0;
  double fluc=sqrt(2.*zeta*temp0*dt);

  calc_force(x,f,a,&(*U),list);
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++){
      v[i][j]+=-zeta*v[i][j]*dt+f[i][j]*dt+fluc*gaussian_rand();
      x[i][j]+=v[i][j]*dt;
    }
  p_boundary(x);
}

void eom_md(double (*v)[dim],double (*x)[dim],double (*f)[dim],double *a,double *U,double dt,int (*list)[Nn]){
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++){
      x[i][j]+=v[i][j]*dt+0.5*f[i][j]*dt*dt;
      v[i][j]+=0.5*f[i][j]*dt;
    }
  calc_force(x,f,a,&(*U),list);
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++){
      v[i][j]+=0.5*f[i][j]*dt;
    }
  p_boundary(x);
}

void output(int k,double (*v)[dim],double U){
  char filename[128];
  double K=0.0;

  std::ofstream file;
  sprintf(filename,"energy.dat");
  file.open(filename,std::ios::app); //append
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++)
      K+=0.5*v[i][j]*v[i][j];
  
  std::cout<< std::setprecision(6)<<k*dtmd<<"\t"<<K/Np<<"\t"<<U/Np<<"\t"<<(K+U)/Np<<std::endl;  
  file<< std::setprecision(6)<<k*dtmd<<"\t"<<K/Np<<"\t"<<U/Np<<"\t"<<(K+U)/Np<<std::endl;
  file.close();
}

void update(double (*x_update)[dim],double (*x)[dim])
{
  for(int i=0;i<Np;i++)
    for(int j=0;j<dim;j++)
      x_update[i][j]=x[i][j];
}

void calc_disp_max(double *disp_max,double (*x)[dim],double (*x_update)[dim])
{
  double dx,dy;
  double disp;
  for(int i=0;i<Np;i++){
    dx=x[i][0]-x_update[i][0];
    dy=x[i][1]-x_update[i][1];
    dx-=L*floor((dx+0.5*L)/L);
    dy-=L*floor((dy+0.5*L)/L);
    disp = dx*dx+dy*dy;
    if(disp > *disp_max)
      *disp_max =disp;
  }
}

void auto_list_update(double *disp_max,double (*x)[dim],double (*x_update)[dim],int (*list)[Nn]){
  static int count=0;
  count++;
  calc_disp_max(&(*disp_max),x,x_update);
  if(*disp_max > skin*skin*0.25){
    list_verlet(list,x);
    update(x_update,x);
    //    std::cout<<"update"<<*disp_max<<" "<<count<<std::endl;
    *disp_max=0.0;
    count=0;
  }
}


int main(){
  double x[Np][dim],x_update[Np][dim],v[Np][dim],f[Np][dim],a[Np];
  int list[Np][Nn];
  double tout=0.0,U,disp_max=0.0;
  int j=0;
  set_diameter(a);
  ini_coord_square(x);
  ini_array(v);
  
  while(j*dtbd < 10.){
    j++;
    auto_list_update(&disp_max,x,x_update,list);
    eom_langevin(v,x,f,a,&U,dtbd,5.0,list);
  }
  
  j=0;
  while(j*dtbd < teq){
    j++;
    auto_list_update(&disp_max,x,x_update,list);
    eom_langevin(v,x,f,a,&U,dtbd,temp,list);
  }
  j=0;
  while(j*dtmd < tmax){
    j++;
    auto_list_update(&disp_max,x,x_update,list);
    eom_md(v,x,f,a,&U,dtmd,list);
    if(j*dtmd >= tout){
      output(j,v,U);
      tout+=1.;
    }
  }  
  return 0;
}
