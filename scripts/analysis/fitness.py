import sys
import shlex
import random
import subprocess

g = sys.argv[1]
seed_start = random.randint(0,1000000)
POPSIZE = 400
MAXGENERATIONS = 198
CROSSOVER = 65
MUTATIONRATE = 35
CONF = 6
PROCS = 4

NPOPS = 1
MIGRATIONRATE = 5
MIGRATIONFREQ = 10

lf = []
for s in range(seed_start,seed_start+100000):
	comm = "mpiexec -n {} ../genalg/genalg {} {} {} {} {} {} {} {} {} {}".format(
			NPOPS,s,g,POPSIZE,MAXGENERATIONS,CROSSOVER,MUTATIONRATE,CONF,PROCS,MIGRATIONFREQ,MIGRATIONRATE)
	# print(comm)
	args = shlex.split(comm)
	p = subprocess.Popen(args,stdout=subprocess.PIPE)
	p.wait()
	fitness = int(p.stdout.read().split(b',')[0])
	lf.append(fitness)
	print(fitness)

# for f in lf:
# 	print(f)
