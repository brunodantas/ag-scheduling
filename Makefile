# Main target

.PHONY: test

test: test.c
	gcc domain/individual.c genalg/population.c genalg/selection.c domain/crossover.c genalg/reinsertion.c domain/graph.c domain/getinput.c domain/list.c genalg/genalg.c test.c -o test -O3