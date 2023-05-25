%matplotlib inline
import math
import matplotlib
%config InlineBackend.figure_format = 'retina'
import matplotlib.cm as cm  # colormap
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as mpatches

plt.rcParams["text.usetex"] =True
plt.rcParams["font.size"] = 30

from matplotlib.collections import PatchCollection
from matplotlib.patches import Circle

resolution = 100 # the number of vertices
Np=1024
L=40.0
temp=[0.200,0.400,0.600,1.000]

patches = [[] for i in range(4)]  #空の二重配列を定義

fig = plt.figure(figsize=(25,6))

for j in range(1,5):
    ax = fig.add_subplot("14{}".format(j))
    x, y,a = np.loadtxt("./Documents/GitHub/2022-simulation-training-main/Lecture7/coord_T{:.3f}_10.dat".format(temp[j-1]), comments='#', unpack=True)
    for i in range(Np):
        circle = mpatches.Ellipse((x[i],y[i]), a[i], a[i])
        patches[j-1].append(circle)
    plt.xlim(0, L)
    plt.ylim(0, L)    
    colors = a
    p = PatchCollection(patches[j-1], cmap=matplotlib.cm.jet, alpha=1.0,ec='k')
    p.set_array(colors)
    p.set_clim(0.8,1.2)
    ax.add_collection(p)
    ax.spines['top'].set_linewidth(3)
    ax.spines['bottom'].set_linewidth(3)
    ax.spines['left'].set_linewidth(3)
    ax.spines['right'].set_linewidth(3)
    plt.tick_params(which='major',width = 1, length = 10)
    plt.tick_params(which='minor',width = 1, length = 5)
 
    plt.xlabel(r"$x/a$",color='k', size=35)
    if(j==1):
        plt.ylabel(r"$y/a$",color='k', size=35)
    plt.title("$T^*={}$".format(temp[j-1]))

plt.subplots_adjust(wspace=0.2, hspace=0.25)
plt.savefig('./Documents/GitHub/2022-simulation-training-main/Lecture7/LJ.pdf',bbox_inches="tight")
plt.show()
