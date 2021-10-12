import matplotlib.pyplot as plt
import matplotlib as mpl
from math import sqrt
import numpy as np

def distance(p1, p2):
    return sqrt((p2[0] - p1[0]) ** 2 + (p2[1] - p1[1]) ** 2)

file1 = open('blobs.txt', 'r')
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

intervals = np.linspace(min(err), max(err) * 1.5, 10)
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

mpl.rcParams["figure.figsize"] = (12, 4)
plt.grid(True)
plt.xticks(xs, labels = xlabels)
plt.bar(xs,heights, label="Blob Center Error")
plt.show()