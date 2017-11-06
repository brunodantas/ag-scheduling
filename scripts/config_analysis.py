import pandas as pd

df = pd.read_csv("configs.csv")

configs = [x for x in set(df.conf)]

#get max means
grafos = [x for x in set(df.grafo)]
max_means = dict()
for g in grafos:
	means = df[df.grafo == g].avg
	max_means[g] = max(means)

#get scores
scores = dict()
for c in configs:
	scores[c] = 0
for i,row in df.iterrows():
	g = row.grafo
	c = row.conf
	s = row.avg / max_means[g]
	scores[c] += s

ngrafos = len(grafos)
print("conf\tscore")
for c in configs:
	scores[c] /= ngrafos
	print("{}\t{}".format(c,scores[c]))
