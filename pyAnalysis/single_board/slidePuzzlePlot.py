# Plots with different scales
# https://matplotlib.org/stable/gallery/subplots_axes_and_figures/two_scales.html

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df = pd.read_csv('output.csv')
showFullDat = False;

if (showFullDat):
    print(df)
else:
    print(df.head(4))
    if (len(df) > 5):
        print(".")
        print(".")
        print(".")
        print(df.tail(2))

x    = df.tile
dat1 = df.activeBoards
dat2 = df.time/1e9
dat3 = df.moves

print('\nSize: ' + str(int(max(x)**(1/2))) + ', Active Boards Max: ' + str(max(dat1)) + ', Time (s): ' + "{:e}".format(max(dat2)) + ', Moves: ' + str(max(dat3)))

fig, ax = plt.subplots(nrows=3,ncols=1)

plt.subplot(3,1,1)
color = 'tab:red'
plt.ylabel('Active Boards', color=color)
plt.tick_params(axis='y', colors=color)
plt.plot(x, dat1, color=color)

plt.subplot(3,1,2)
color = 'tab:blue'
plt.ylabel('Total Time (s)', color=color)
plt.tick_params(axis='y', colors=color)
plt.plot(x, dat2, color=color)

plt.subplot(3,1,3)
color = 'tab:green'
plt.xlabel('Tile')
plt.ylabel('Number of Moves', color=color)
plt.tick_params(axis='y', colors=color)
plt.plot(x, dat3, color=color)

plt.show()
