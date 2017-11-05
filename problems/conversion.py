#convert morady's problems into my format

import sys

def convert(p,lines):
	nverts, nedges = lines[1].split()
	lines2 = []

	i=2
	while len(lines[i].split()) == 2:
		_,w = lines[i].split()
		lines2.append(w)
		i += 1

	lines2.append(nedges)

	while i < len(lines):
		x,y,w = lines[i].split()
		l = "{} {} {}".format(x,y,w)
		lines2.append(l)
		i += 1

	out = "converted/TG{}.txt".format(nverts)
	with open(out,'w+') as f:
		for l in lines2:
			f.write(l+'\n')

if __name__ == '__main__':
	for p in sys.argv[1:]:
		with open(p,'r') as f:
			lines = f.readlines()
		convert(p,lines)