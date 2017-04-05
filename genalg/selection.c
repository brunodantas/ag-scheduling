#include "genalg.h"


//seleciona um indivíduo para reprodução
Individual* tournament()
{
	Individual *best,*ind;
	int i,r;

	r = rand()%POPSIZE;
	best = population[r];

	for(i=1;i<tournamentsize;++i)
	{
		r = rand()%POPSIZE;
		ind = population[r];
		if(ind->fitness < best->fitness)
			best = ind;
	}
	return best;
}


Individual* random_selection()
{
	int r;
	r = rand()%POPSIZE;
	return population[r];
}