#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>

#define L 40.0
#define a 1.0
#define Np 512
#define dim 2

void ini_square(double (*x)[dim]){
  int num_x = (int)sqrt(Np)+1;
  int num_y = (int)sqrt(Np)+1;
  int i,j,k=0;
  for(j=0;j<num_y;j++){
    for(i=0;i<num_x;i++){
      x[i+num_x*j][0] = i*L/(double)num_x;
      x[i+num_x*j][1] = j*L/(double)num_y;
      k++;
      if(k==Np){
        break;
      }
    }
    if(k==Np){
        break;
    }
  }
}

void ini_hex(double (*x)[dim]){
  int num_x = (int)sqrt(Np)+1;
  int num_y = (int)sqrt(Np)+1;
  int i,j,k=0;
  double shift;
  for(j=0;j<num_y;j++){
    for(i=0;i<num_x;i++){
      shift=(double)j*0.5-j/2;
      x[i+num_x*j][0] = (shift+i)*L/(double)num_x;
      x[i+num_x*j][1] = j*L/(double)num_y;
      k++;
      if(k==Np){
        break;
      }
    }
    if(k==Np){
        break;
    }
  }
}


void output(double (*x1)[dim],double (*x2)[dim]){
  char filename[128];
  std::ofstream file;
  sprintf(filename,"coord_square_L%.1fN%d.dat",L,Np);
  file.open(filename);
  for(int i=0;i<Np;i++)
    file<<x1[i][0]<<"\t"<<x1[i][1]<<"\t"<< a <<std::endl;
  file.close();

  sprintf(filename,"coord_hex_L%.1fN%d.dat",L,Np);
  file.open(filename);
  for(int i=0;i<Np;i++)
    file<<x2[i][0]<<"\t"<<x2[i][1]<<"\t"<< a <<std::endl;
  file.close();
}

int main(){
  double  (*x1)[dim] = new double[Np][dim];
  double  (*x2)[dim] = new double[Np][dim];
  ini_square(x1);
  ini_hex(x2);
  output(x1,x2);
  delete[] x1;
  delete[] x2;
  return 0;
}
