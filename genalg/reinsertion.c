#include "genalg.h"


poplist l1,l;


//une duas listas ordenadas
static inline poplist mergepop(poplist a,poplist b)
{
	int i,j,s=POPSIZE+NEXTGENSIZE;

	for(i=0,j=0,l->size = 0;l->size < s;++l->size)
	{
		if(i==a->size || j==b->size)
			break;
		if(a->info[i]->fitness < b->info[j]->fitness)
		{
			l->info[l->size] = a->info[i];
			++i;
		}
		else
		{
			l->info[l->size] = b->info[j];
			++j;
		}
	}
	for(;i<a->size && l->size < s;++i,++l->size)
		l->info[l->size] = a->info[i];

	for(;j<b->size && l->size < s;++j,++l->size)
		l->info[l->size] = b->info[j];

	return l;
}


static inline void reins(Population nextgen,int size)
{
	int i;
	Population aux;
	if(init)
	{
		init=0;
		l1 = malloc(2*sizeof(struct pl));

		l = malloc(sizeof(struct pl));
		l->info = malloc((POPSIZE+NEXTGENSIZE)*sizeof(Individual*));
	}
	
	l1[0].info = population;
	l1[0].size = size;
	l1[1].info = nextgen;
	l1[1].size = NEXTGENSIZE;

	l = mergepop(&l1[0],&l1[1]);
	aux = population;
	population = l->info;
	l->info = aux;

	for(i=POPSIZE;i<POPSIZE+NEXTGENSIZE;i++)
	{
		recycleindividual(population[i]);
	}
}


//elimina piores indivíduos entre pais e filhos
void bestreinsertion(Population nextgen)
{
	reins(nextgen,POPSIZE);
}