import matplotlib
import matplotlib.pyplot as plt
%matplotlib inline
#図の解像度が上がる
%config InlineBackend.figure_format = 'retina'
import numpy as np
#Texフォント
plt.rcParams["text.usetex"] =True 
#図全体のサイズやアスペクト比を変える
fig = plt.figure(figsize=(7,7))
#複数の図を並べる時ここを変える
ax = fig.add_subplot(111)
#各自ファイルのパスを変えること
x, y  = np.loadtxt("./Documents/GitHub/2022-simulation-training-main/Lecture1/coord100000.dat", comments='#', unpack=True)
plt.plot(x, y, "o",markersize=0.5,color="b",label=r"$x^2+y^2\leq1$")

#図の書式設定
plt.tick_params(which='major',width = 1, length = 10)
plt.tick_params(which='minor',width = 1, length = 5)
ax.spines['top'].set_linewidth(3)
ax.spines['bottom'].set_linewidth(3)
ax.spines['left'].set_linewidth(3)
ax.spines['right'].set_linewidth(3)
plt.xlabel(r"$x$",color='k', size=30)
plt.ylabel(r"$y$",color='k', size=30)
plt.xticks(color='k', size=25)
plt.yticks(color='k', size=25)
#図の凡例の有無や位置，サイズを調整
plt.legend(ncol=1, loc=1, borderaxespad=0, fontsize=25,frameon=True)
#図のマージン設定
plt.subplots_adjust(wspace=0.0, hspace=0.25)
#各グラフのアスペクト比を1:1にする
ax.set_aspect('equal', adjustable='box')
#各自ファイルのパスを変えること．
plt.savefig('./Documents/GitHub/2022-simulation-training-main/Lecture1/coord.png')
plt.savefig('./Documents/GitHub/2022-simulation-training-main/Lecture1/coord.pdf')
