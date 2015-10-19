# Main target

.PHONY: test

test: test.c
	gcc individual.c population.c graph.c getinput.c list.c test.c -o test -O1