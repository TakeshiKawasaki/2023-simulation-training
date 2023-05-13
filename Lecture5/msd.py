#第4回自主課題 (1)
import matplotlib
import matplotlib.pyplot as plt
%matplotlib inline
%config InlineBackend.figure_format = 'retina'
import matplotlib.cm as cm  # colormap
import numpy as np
import math
plt.rcParams["text.usetex"] =True 
fig = plt.figure(figsize=(12,8))
temp=[0.01,0.1,1]

ax1 = fig.add_subplot(111)
for i in range(0,3):
    t,msd= np.loadtxt("./Documents/GitHub/2022-simulation-training-main/Lecture5/msd_dt0.010T{:.3f}.dat".format(temp[i]), comments='#', unpack=True)
    plt.plot(t,msd, "o" ,markersize=10,color=cm.jet(0.2+i/3),label=r"$T^*={:.3f}$".format(temp[i]))

    t= np.linspace(0.01, 20, 1000) 
    f= 2*3*temp[i]*(t+np.exp(-t)-1)
    plt.plot(t,f, "--",markersize=3,linewidth = 3.0, color="k", label=r"$T^*={:.3f}$ theory".format(temp[i]))
    
plt.xscale('log')
plt.yscale('log')

plt.tick_params(which='major',width = 1, length = 10)
plt.tick_params(which='minor',width = 1, length = 5)
ax1.spines['top'].set_linewidth(3)
ax1.spines['bottom'].set_linewidth(3)
ax1.spines['left'].set_linewidth(3)
ax1.spines['right'].set_linewidth(3)
plt.xlabel(r"$t/t_0$",color='k', size=30)
plt.ylabel(r"$\langle \Delta \mathbf{r}(t)^2\rangle/a^2$",color='k', size=30)
plt.xticks(color='k', size=30)
plt.yticks(color='k', size=30)

plt.legend(ncol=1, loc=4, borderaxespad=0, fontsize=20,frameon=False)
#################################
plt.subplots_adjust(wspace=0.5, hspace=0.25)
plt.savefig('./Documents/GitHub/2022-simulation-training-main/Lecture5/msd.pdf')
plt.show()
