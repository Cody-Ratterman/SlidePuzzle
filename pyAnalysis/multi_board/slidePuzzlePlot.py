# Plots with different scales
# https://matplotlib.org/stable/gallery/subplots_axes_and_figures/two_scales.html

import pandas as pd
import glob
import numpy as np
import matplotlib.pyplot as plt

files = glob.glob("*.csv")
activeBoards = []
time = []
moves = []
for f in range(len(files)):
    df = pd.read_csv(files[f])
    moves.append([])
    time.append([])
    activeBoards.append([])
    
    for row in range(len(df)):
        activeBoards[f].append(df["activeBoards"].iloc[row])
        time[f].append(df["time"].iloc[row])
        moves[f].append(df["moves"].iloc[row])
    
    del df

# --- Custom Axis Labels
x = []
for i in range(1,len(moves[0])+1):
    x.append(i)

labels = ['Start']
for i in range(1,len(moves[0])-1):
     labels.append(str(i))
labels.append('End')

# --- Tile vs Active Boards Plot ---
data = [[]]
for i in range(len(activeBoards[0])):
    data.append([])
    for j in range(len(activeBoards)):
        data[i].append(activeBoards[j][i])

fig, ax = plt.subplots()
ax.set_title('Active Boards Distribution by Tile Number')
ax.boxplot(data)
plt.xticks(x, labels, rotation='vertical')

plt.savefig('plots/activeBoards.png')
del data

# --- Tile vs Total Time Plot ---
data = [[]]
for i in range(len(time[0])):
    data.append([])
    for j in range(len(time)):
        data[i].append(time[j][i])

fig, ax = plt.subplots()
ax.set_title('Total Time Distribution by Tile Number')
ax.boxplot(data)
plt.xticks(x, labels, rotation='vertical')

plt.savefig('plots/time.png')
del data
    
# --- Tile vs Moves Box Plot ---
data = [[]]
for i in range(len(moves[0])):
    data.append([])
    for j in range(len(moves)):
        data[i].append(moves[j][i])

fig, ax = plt.subplots()
ax.set_title('Moves Distribution by Tile Number')
ax.boxplot(data)
plt.xticks(x, labels, rotation='vertical')

plt.savefig('plots/moves.png')
del data
