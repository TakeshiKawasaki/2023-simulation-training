import numpy as np
import sys

zeta=[10,2,1,0.1]
dt=[0.0001,0.001,0.01,0.1]

def damp_osci(dt,zeta):
    v=0.
    x=1.
    t=0.
    i=0
    out=0.
    f = open("./Lecture4/damp_osci_dt{:.4f}_zeta{:.1f}.dat".format(dt,zeta), 'w')
    while i < (int)(50./dt): 
        v -= zeta*v*dt+x*dt
        x+=v*dt
        i+=1
        if(i*dt >=out):
            print("{:.4f} {:.20f} {:.20f}".format(i*dt,v,x))
            f.write("{:.4f} {:.20f} {:.20f}\n".format(i*dt,v,x))
            out+=0.5
    f.close()
    


for i in range(0,4):
    for j in range(0,4):
        print("dt={:.5f}".format(dt[i]))
        damp_osci(dt[i],zeta[j]) 
