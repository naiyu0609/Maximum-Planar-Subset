import numpy as np
import matplotlib.pyplot as plt
import os
# ==========================================
# 圓的基本資訊
# 1.圓半徑
r = 2.0
# 2.圓心座標
a, b = (0., 0.)
pointx = []
pointy = []
total = 500

with open(f'./{total}.out') as f:
    number = f.readline()
    number = int(number.strip())
    for line in f:
        line = line.strip()
        pointx.append(int(line.split(' ')[0]))
        pointy.append(int(line.split(' ')[1]))
        

theta = np.arange(0, 2*np.pi, 0.01)
x = a + r * np.cos(theta)
y = b + r * np.sin(theta)
fig = plt.figure() 
axes = fig.add_subplot(111) 
axes.plot(x, y)
theta = 2*np.pi/total
for i in range(number):
    x = []
    y = []
    x.append(a + r * np.sin(pointx[i]*theta))
    y.append(b + r * np.cos(pointx[i]*theta))
    x.append(a + r * np.sin(pointy[i]*theta))
    y.append(b + r * np.cos(pointy[i]*theta))
    axes.plot(x, y)
axes.axis('equal')
plt.show()