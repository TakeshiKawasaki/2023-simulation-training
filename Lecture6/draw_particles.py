#第５回自主課題
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
import numpy as np

resolution = 100 # the number of vertices
Np=512
L=40.0
patches = []

fig = plt.figure(figsize=(10,10))
ax = fig.add_subplot(111)

x, y,r = np.loadtxt("./Lecture6/coord_square_L40.0N512.dat", comments='#', unpack=True)

for i in range(Np):
    circle = mpatches.Ellipse((x[i],y[i]), r[i], r[i])  # 楕円の中心座標，長軸　短軸　（今回は真円）
    patches.append(circle)

plt.xlim(0, L)
plt.ylim(0, L)    

colors = x
p = PatchCollection(patches, cmap=matplotlib.cm.jet, alpha=1.0,ec='k')
p.set_array(colors)
#####color range ####
p.set_clim(0,L)
#####
ax.add_collection(p)

C=plt.colorbar(p,shrink=0.6) # shirink:  controlling the size of color bar: 1.0 is maximum
C.set_label(r"$x$", fontsize=30) # color bar label

ax.spines['top'].set_linewidth(3)
ax.spines['bottom'].set_linewidth(3)
ax.spines['left'].set_linewidth(3)
ax.spines['right'].set_linewidth(3)
plt.tick_params(which='major',width = 1, length = 10)
plt.tick_params(which='minor',width = 1, length = 5)
plt.xticks(color='k', size=30)
plt.yticks(color='k', size=30)
plt.xlabel(r"$x$",color='k', size=35)
plt.ylabel(r"$y$",color='k', size=35)

ax.set_aspect('equal')

plt.savefig('./Lecture6/square.pdf',bbox_inches="tight")
plt.show()