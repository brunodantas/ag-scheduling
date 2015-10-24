#include "genalg.h"


//seleciona um indivíduo para reprodução
Individual* roullete()
{
	int i;
	int worstfitness;
	Individual *p;
	long j, roulletesize = 0;

	worstfitness = population[POPSIZE-1]->fitness + 1;

	for(i=0;i<POPSIZE;++i)
		roulletesize += worstfitness - population[i]->fitness;

	j = rand()%roulletesize;
	for(i=0;i<POPSIZE;++i)
	{
		p = population[i];
		j -= worstfitness - p->fitness;
		if(j<0)
			break;
	}
	return p;
}


//seleciona um indivíduo para reprodução
Individual* tournament(int tournamentsize)
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


Individual* tournament3()
{
	return tournament(3);
}


Individual* tournament2()
{
	return tournament(2);
}