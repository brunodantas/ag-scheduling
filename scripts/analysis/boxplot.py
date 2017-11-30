import numpy as np
import matplotlib.pyplot as plt

fl, fl2 = [], []
with open("fitness.txt") as file:
    for line in file:
        fl.append(int(line))

with open("fitness8pop.txt") as file:
    for line in file:
        fl2.append(int(line))


fl = np.array(fl)
fl2 = np.array(fl2)

best = np.min(fl)
worst = np.max(fl)
fl_unique, counts = np.unique(fl, return_counts=True)
conv = dict(zip(fl_unique, counts))[best]
bins = len(fl_unique)
print(bins)
print("best = {}\nconv = {}\nworst = {}\nmean = {}\nstd = {}".format(
    best, conv, worst, np.mean(fl), np.std(fl)))
# n, bins, _ = plt.hist(fl,bins,range=(best,worst))
plt.ylim(.5,2.5)
plt.xlabel('fitness')
plt.xticks(range(best, worst + 1))
plt.boxplot([fl, fl2], 0, None, 0, widths = .5)
plt.show()
