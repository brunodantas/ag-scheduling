#include "genalg.h"


void initrecyclelist()
{
	recyclelist = malloc((POPSIZE+NEXTGENSIZE)*sizeof(Individual*));
	recyclelistsize = 0;
}


Individual* getrecycledindividual()
{
	Individual* ind;
	if(recyclelistsize>0)
	{
		recyclelistsize--;
		return recyclelist[recyclelistsize];
	}
	return allocateindividual();
}


void recycleindividual(Individual* ind)
{
	recyclelist[recyclelistsize] = ind;
	recyclelistsize++;
}


void recyclepopulation()
{
	int i;
	for(i=0;i<POPSIZE;i++)
		recycleindividual(population[i]);
}