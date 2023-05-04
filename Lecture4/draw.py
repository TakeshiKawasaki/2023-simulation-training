import matplotlib
import matplotlib.pyplot as plt
%matplotlib inline
%config InlineBackend.figure_format = 'retina'
import numpy as np
#plt.rcParams["text.usetex"] =True

plt.rcParams['font.family'] = 'Arial' #使用するフォント名
plt.rcParams["font.size"] = 25

fig = plt.figure(figsize=(25,20))

dt=[0.0001,0.001,0.01,0.1]
zeta=[10,2,1,0.1]

for j in range(1,5):
    ax1 = fig.add_subplot(2,2,j)
    t, v0,x0  = np.loadtxt("./Lecture4/damp_osci_dt{:.4f}_zeta{:.1f}.dat".format(dt[j-1],zeta[0]), comments='#', unpack=True)
    t, v1,x1  = np.loadtxt("./Lecture4/damp_osci_dt{:.4f}_zeta{:.1f}.dat".format(dt[j-1],zeta[1]), comments='#', unpack=True)
    t, v2,x2  = np.loadtxt("./Lecture4/damp_osci_dt{:.4f}_zeta{:.1f}.dat".format(dt[j-1],zeta[2]), comments='#', unpack=True)
    t, v3,x3  = np.loadtxt("./Lecture4/damp_osci_dt{:.4f}_zeta{:.1f}.dat".format(dt[j-1],zeta[3]), comments='#', unpack=True)
    plt.title("$\Delta t/t_s ={:.4f}$".format(dt[j-1]),size=20)
    plt.plot(t, x0, "o-",markersize=5,color="b",label=r"$t_s/t_d={:.1f}$".format(zeta[0]))
    plt.plot(t, x1, "o-",markersize=5,color="g",label=r"$t_s/t_d={:.1f}$".format(zeta[1]))
    plt.plot(t, x2, "o-",markersize=5,color="r",label=r"$t_s/t_d={:.1f}$".format(zeta[2]))
    plt.plot(t, x3, "o-",markersize=5,color="m",label=r"$t_s/t_d={:.1f}$".format(zeta[3]))

    #########
    plt.tick_params(which='major',width = 1, length = 10)
    plt.tick_params(which='minor',width = 1, length = 5)
    ax1.spines['top'].set_linewidth(3)
    ax1.spines['bottom'].set_linewidth(3)
    ax1.spines['left'].set_linewidth(3)
    ax1.spines['right'].set_linewidth(3)
    plt.ylabel(r"$x(t)/a$",color='k', size=35)
    plt.xlabel(r"$t/t_s$",color='k', size=35)

    plt.legend(ncol=1, loc=1, borderaxespad=0, fontsize=25,frameon=False)
    #################################
    #図のマージン設定
    plt.subplots_adjust(wspace=0.3, hspace=0.35)

plt.savefig('./Lecture4/damp_osci.png')
plt.savefig('./Lecture4/damp_osci.pdf')
