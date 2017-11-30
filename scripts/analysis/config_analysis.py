import pandas as pd
import sys

file = sys.argv[1]
df = pd.read_csv(file)
df2 = pd.read_csv("opt.csv")

configs = [x for x in set(df.conf)]

#get optimal values
grafos = [x for x in df2.grafo]
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
for c in configs:
	best[c] = 0
	avg[c] = 0
	conv[c] = 0
	worst[c] = 0
for i,row in df.iterrows():
	g = row.grafo
	c = row.conf
	b = (row.best - opt[g]) / opt[g]
	s = (row.avg - opt[g]) / opt[g]
	dev.append(s)
	best[c] += b
	avg[c] += s
	conv[c] += row.conv
	worst[c] += (row.worst - opt[g]) / opt[g]
df = df.assign(deviation=dev)

df.to_csv("out"+file)

ngrafos = len(grafos)
print("conf\tconv\tbest\tavg\tworst")
for c in configs:
	best[c] /= ngrafos
	conv[c] /= ngrafos
	avg[c] /= ngrafos
	worst[c] /= ngrafos
	print("{}\t{}\t{}\t{}\t{}".format(c,conv[c],best[c],avg[c],worst[c]))
