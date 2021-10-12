import matplotlib.pyplot as plt
import matplotlib as mpl
from math import sqrt
import numpy as np

bars = 20

def distance(p1, p2):
    return sqrt((p2[0] - p1[0]) ** 2 + (p2[1] - p1[1]) ** 2)

files = ['blobs.txt', 'regions.txt', 'ellipse.txt']
titles = ["Blob Detection Error Plot", "Region Detection Error Plot", "Ellipse Detection Error Plot"]
labels = ["Blob Error", "Region Error", "Ellipse Error"]
colors = [(0,0,1,0.5), (1,0,0,0.5), (0,1,0,0.5)]
offset = [-1, 0, 1]

fig = plt.figure(figsize=(12, 10))
grid = fig.add_gridspec(3,3)
ax1 = fig.add_subplot(grid[0, :])
ax2 = fig.add_subplot(grid[1, :])
ax3 = fig.add_subplot(grid[2, :])

axes = [ax1, ax2, ax3]

fig2 = plt.figure(figsize=(12, 4))
ax4 = fig2.add_subplot(1,1,1)

for f in range(3):
    file1 = open(files[f], 'r')
    lines = file1.readlines()

    inner_c = []
    outer_c = []
    for i in range(len(lines)):
        c = lines[i].split(';')
        if i % 2 == 0:
            inner_c.append([float(c[0]), float(c[1])])
        if i % 2 == 1:
            outer_c.append([float(c[0]), float(c[1])])

    eps = 10
    pairs = []
    err = []
    for i in range(len(inner_c)):
        c = inner_c[i]
        for j in range(len(outer_c)):
            dist = distance(c, outer_c[j])
            if dist < eps:
                pairs.append([c, outer_c[j]])
                err.append(dist)
    print(err)

    if f == 0:
        intervals = np.linspace(min(err), max(err) * 1.5, bars)
        intervals = list(intervals)
        intervals = [0.0] + intervals

    heights = [0 for i in intervals[1:]]
    xs = list(range(len(heights)))

    xlabels = ["[{:.2f}-{:.2f})".format(intervals[i-1], intervals[i]) for i in range(1, len(intervals))]

    for i in range(len(intervals) - 1):
        minVal = intervals[i]
        maxVal = intervals[i + 1]
        for e in err:
            if e >= minVal and e < maxVal:
                heights[i] += 1

    print(intervals)
    print(heights, xs)

    axes[f].set_title(titles[f])
    axes[f].set_xticks(xs)
    axes[f].set_xticklabels(xlabels, rotation=90)

    axes[f].bar(xs, heights, color=colors[0])
    width = 0.3
    ax4.bar(np.array(xs) + (width)*(offset[f]), heights, width=width, color=colors[f], label=labels[f])

ax4.set_title("Error Plot")
ax4.set_xticks(xs)
ax4.set_xticklabels(xlabels, rotation=90)
fig2.legend()

fig.tight_layout()
fig2.tight_layout()
plt.show()