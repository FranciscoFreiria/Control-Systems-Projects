# -*- coding: utf-8 -*-
"""
Francisco Freiria e João Morais

This is a "temporary" script file.
"""

import numpy as np
import matplotlib.pyplot as plt

datax = np.load('data1_x.npy')
np.save('data1_x.npy', datax)
print (datax)
datay = np.load('data1_y.npy')
np.save('data1_y.npy', datay)
print (datay)
plt.scatter(datax, datay, label = "line 1")
z = np.polyfit(datax, datay, 1)
plt.plot(z, label = "line 1")
plt.show()