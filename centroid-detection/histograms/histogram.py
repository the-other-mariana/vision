import matplotlib.pyplot as plt
from math import sqrt

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
for i in range(len(inner_c)):
    c = inner_c[i]
    for j in range(len(outer_c)):
        dist = distance(c, outer_c[j])
        if dist < eps:
            pairs.append([c, outer_c[j]])
print(pairs)