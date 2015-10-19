//operações na população


#include "genalg.h"


//gera população inicial
Population* initpopulation()
{
	Individual* ind;
	population = malloc((POPSIZE+NEXTGENSIZE)*sizeof(Individual*));
	int i,j;
	for(i=0;i<POPSIZE;++i)
	{
		ind = newindividual();
		populationinsert(population,ind,i);
	}
	return &population;
}


//insere indivíduo ordenado pela aptidão
void populationinsert(Population population,Individual *ind,int size)
{
	int pos,i;
	evaluate(ind);
	for(pos=0;pos<size;++pos)
	{
		if(population[pos]->fitness > ind->fitness)
			break;
	}
	for(i=size;i>pos;i--)
		population[i] = population[i-1];

	population[pos] = ind;
}