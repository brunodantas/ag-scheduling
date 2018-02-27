import pandas as pd
import sys

file = sys.argv[1]
df2 = pd.read_csv("opt.csv")

df = pd.read_csv(file)
pops = [x for x in set(df.npops)]

#get optimal values
grafos = [x for x in set(df2.grafo)]
opt = dict()
for g in grafos:
	opt[g] = int(df2[df2.grafo == g].best)

#overwrite convergence
conv = []
for i,row in df.iterrows():
	g = row.grafo
	c = row.conv
	if row.best > opt[g]:
		c = 0
	conv.append(c)
df.conv = conv

#get scores
dev = []
best = dict()
avg = dict()
conv = dict()
worst = dict()
time = dict()
for p in pops:
	conv[p] = 0
	best[p] = 0
	avg[p] = 0
	worst[p] = 0
	time[p] = 0
for i,row in df.iterrows():
	g = row.grafo
	p = row.npops
	b = (row.best - opt[g]) / opt[g]
	a = (row.avg - opt[g]) / opt[g]
	w = (row.worst - opt[g]) / opt[g]
	dev.append(a)
	conv[p] += row.conv
	best[p] += b
	avg[p] += a
	worst[p] += w
	time[p] += row.time
df = df.assign(deviation=dev)

df.to_csv("out"+file)

ngrafos = len(grafos)
print("pops\tconv\tbest\tavg\tworst\ttime\tspeedup")
for c in pops:
	conv[c] /= ngrafos
	best[c] /= ngrafos
	avg[c] /= ngrafos
	worst[c] /= ngrafos
	time[c] /= ngrafos
	speedup = time[1] / time[c]
	print("{}\t{}\t{}\t{}\t{}\t{}\t{}".format(c,conv[c],best[c],avg[c],worst[c],time[c],speedup))
