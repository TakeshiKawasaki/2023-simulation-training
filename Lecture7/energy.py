%config InlineBackend.figure_format = 'retina'
%matplotlib inline
import matplotlib.cm as cm  # colormap
import matplotlib.pyplot as plt
import numpy as np
plt.rcParams["text.usetex"] =True
plt.rcParams["font.size"] = 25
x, y1, y2, y3 = np.loadtxt("./Documents/GitHub/2022-simulation-training-main/Lecture7/energy.dat", comments='#', unpack=True)

fig = plt.figure(figsize=(10,6))
ax = fig.add_subplot(111)
plt.plot(x, y1, "o-", color=cm.jet(0.5/4), linewidth = 2.0,label="$K/N\epsilon$")
plt.plot(x, y2, "o-", color=cm.jet(2.0/4), linewidth = 2.0,label="$U/N\epsilon$")
plt.plot(x, y3-0.75, "o-", color=cm.jet(3.5/4), linewidth = 2.0,label="$(U+K)/N\epsilon-0.75$")

plt.xlim(0, 30)
plt.xlabel("$t/t_0$",color='k', size=40)
plt.ylabel("Energy",color='k', size=40)
plt.subplots_adjust(wspace=0.4, hspace=0.4)
#目盛設定

plt.tick_params(which='major',width = 1, length = 10)
plt.tick_params(which='minor',width = 1, length = 5)
ax.spines['top'].set_linewidth(2)
ax.spines['bottom'].set_linewidth(2)
ax.spines['left'].set_linewidth(2)
ax.spines['right'].set_linewidth(2)
plt.legend(ncol=1, loc=4, borderaxespad=0, fontsize=20,frameon=True)
##


plt.savefig('./Documents/GitHub/2022-simulation-training-main/Lecture7/energy.pdf',bbox_inches="tight")
plt.show()
