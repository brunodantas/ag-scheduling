import numpy as np
import matplotlib.pyplot as plt

fl = []
with open("fitness.txt") as file:
	for line in file:
		fl.append(int(line))

fl = np.array(fl)
best = np.min(fl)
worst = np.max(fl)
fl_unique, counts = np.unique(fl, return_counts=True)
conv = dict(zip(fl_unique,counts))[best]
bins = len(fl_unique)
print(bins)
print("best = {}\nconv = {}\nworst = {}\nmean = {}\nstd = {}".format(
	best,conv,worst,np.mean(fl),np.std(fl)))
n, bins, _ = plt.hist(fl,bins,range=(best,worst))
plt.xlabel('fitness')
plt.xticks(range(best,worst+1))
plt.show()