import matplotlib.pyplot as plt
import matplotlib as mpl
from math import sqrt
import numpy as np

bars = 20
folder = 'files/'
out = 'out/'

def distance(p1, p2):
    return sqrt((p2[0] - p1[0]) ** 2 + (p2[1] - p1[1]) ** 2)



files = [['blobscenter.txt', 'regionscenter.txt', 'ellipsecenter.txt'],
         ['blobsfront45.txt', 'regionsfront45.txt', 'ellipsefront45.txt'],
         ['blobsside4504.txt', 'regionsside4502.txt', 'ellipseside4502.txt']]

tests = ['Center Pattern', 'Front-tilt Pattern', 'Side-tilt Pattern']

titles = ["Blob Detection Error Plot", "Region Detection Error Plot", "Ellipse Detection Error Plot"]
labels = ["Blob Error", "Region Error", "Ellipse Error"]
colors = [(0,0,1,0.5), (1,0,0,0.5), (0,1,0,0.5)]
offset = [-1, 0, 1]

figs = [None, None, None]

for a in range(3):
    errors = []
    global_err = []
    figs[a] = plt.figure(figsize=(12, 10))
    figs[a].suptitle(f"Error Distributions Per Algorithm ({tests[a]})", fontsize="x-large")

    grid = figs[a].add_gridspec(3,3)
    ax1 = figs[a].add_subplot(grid[0, :])
    ax2 = figs[a].add_subplot(grid[1, :])
    ax3 = figs[a].add_subplot(grid[2, :])

    axes = [ax1, ax2, ax3]

    fig2 = plt.figure(figsize=(12, 4))
    ax4 = fig2.add_subplot(1,1,1)

    for f in range(3):
        file1 = open(folder + files[a][f], 'r')
        lines = file1.readlines()

        centers = []
        for i in range(len(lines)):
            c = lines[i].split(';')
            centers.append([float(c[0]), float(c[1])])

        eps = 10
        pairs = []
        err = []
        for i in range(len(centers)):
            c = centers[i]
            for j in range(len(centers)):
                if j == i:
                    continue
                dist = distance(c, centers[j])
                if dist < eps:
                    pairs.append([c, centers[j]])
                    err.append(dist)
                    errors.append(dist)
        print(f"{labels[f]}", err)
        global_err.append(err)

    for f in range(3):

        #if f == 0:
        intervals = np.linspace(min(errors), max(errors) * 1.5, bars)
        intervals = list(intervals)
        intervals = [0.0] + intervals

        heights = [0 for i in intervals[1:]]
        xs = list(range(len(heights)))

        xlabels = ["[{:.2f}-{:.2f})".format(intervals[i-1], intervals[i]) for i in range(1, len(intervals))]

        for i in range(len(intervals) - 1):
            minVal = intervals[i]
            maxVal = intervals[i + 1]
            for e in global_err[f]:
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

    ax4.set_title(f"Error Distribution Condensed ({tests[a]})")
    ax4.set_xticks(xs)
    ax4.set_xticklabels(xlabels, rotation=90)
    fig2.legend()

    figs[a].tight_layout()
    fig2.tight_layout()

    figs[a].savefig(out + 'grid-' + tests[a].split()[0].lower() + '.png', dpi=500)
    fig2.savefig(out + 'condensed-' + tests[a].split()[0].lower() + '.png', dpi=500)
plt.show()