import matplotlib.pyplot as plt
%matplotlib inline
# グラフの解像度が上がる
%config InlineBackend.figure_format = 'retina'
price = [100, 250, 380, 500, 700]
number = [1, 2, 3, 4, 5]

# グラフを書く
plt.plot(price, number)

# グラフのタイトル
plt.title("price / number")

# x軸のラベル
plt.xlabel("price")

# y軸のラベル
plt.ylabel("number")

# 表示する
plt.show()
