%matplotlib inline
import math
import matplotlib
%config InlineBackend.figure_format = 'retina'
import matplotlib.cm as cm  # colormap
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as mpatches

#plt.rcParams["text.usetex"] =True
plt.rcParams["font.size"] = 30
plt.rcParams['font.family'] = 'Arial' #使用するフォント名

from matplotlib.collections import PatchCollection
from matplotlib.patches import Circle
import numpy as np

resolution = 100 # the number of vertices
Np=1024
L=40.0
temp=0.200
patches = []

fig = plt.figure(figsize=(10,10))
ax = fig.add_subplot(111)

x, y,a = np.loadtxt("./Lecture8/coord_mc_T{:.3f}_50.dat".format(temp), comments='#', unpack=True)

for i in range(Np):
    circle = mpatches.Ellipse((x[i],y[i]), a[i], a[i])
    patches.append(circle)

plt.xlim(0, L)
plt.ylim(0, L)    

colors = a
p = PatchCollection(patches, cmap=matplotlib.cm.jet, alpha=1.0,ec='k')
p.set_array(colors)
#####color range ####
p.set_clim(0.8,1.2)
#####
ax.add_collection(p)

C=plt.colorbar(p,shrink=0.6) 
C.set_label(r"diameter$/a$", fontsize=30) 

ax.spines['top'].set_linewidth(3)
ax.spines['bottom'].set_linewidth(3)
ax.spines['left'].set_linewidth(3)
ax.spines['right'].set_linewidth(3)
plt.tick_params(which='major',width = 1, length = 10)
plt.tick_params(which='minor',width = 1, length = 5)
plt.xticks(color='k', size=30)
plt.yticks(color='k', size=30)
plt.xlabel(r"$x/a$",color='k', size=35)
plt.ylabel(r"$y/a$",color='k', size=35)

ax.set_aspect('equal')

plt.savefig('./Lecture8/LJ_mc_{:.3f}.pdf'.format(temp),bbox_inches="tight")
plt.show()
