#include <stdio.h> // for printf, etc                                                                                                                                       
#include <stdlib.h> // for rand(), etc                                                                                                                                      
#include <math.h> // for sin(),cos(), etc                                                                                                                                   
#include <iostream>// for cout, etc 
#include <iomanip>// for setprecision()
#include <fstream> // for ifstream/ofstream                                                                                                                                 
#include <time.h>// for time(NULL), etc                                                                                                                                     

int main(void){
  int i, count = 0, max = 1e+5;
  double x,y,z,pi;
  char fname[128];
  std::ofstream file;
  srand(time(NULL));  // "time(NULL)" as a seed of ramdom number
  sprintf(fname,"coord%d.dat",max); // Define the file name for fname[128]
  file.open(fname); // "file" with the name of fname[128]
  for(i=0;i<max;i++){
    x = (double)rand()/RAND_MAX;
    y = (double)rand()/RAND_MAX;
    z = x*x + y*y;
    if(z<1){
      count++;
      file << x <<" "<<y <<std::endl;
    }
  }
    file.close();
    pi = (double)count / max * 4.;
    printf("%.20f\n",pi); //by C, %.20f -- Displaying with 20 decimal precision
    std::cout<< std::setprecision(21) <<  pi  << std::endl; // by C++
 
    return 0;
}
