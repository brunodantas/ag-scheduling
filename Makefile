# Main target

.PHONY: test genalg

genalg: genalg.c
	gcc genalg.c individual.c population.c selection.c crossover.c reinsertion.c graph.c getinput.c list.c -o genalg -O1

#test: test.c
#	gcc individual.c population.c selection.c crossover.c reinsertion.c graph.c getinput.c list.c test.c -o test -O1