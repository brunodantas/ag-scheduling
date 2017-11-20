#include "genalg.h"


poplist l1,l;


//elimina toda a população anterior menos o melhor
void elite1reinsertion(Population nextgen)
{
	int i,j,flag=1;

	Individual* s = population[0];

	for(i=0,j=0;i<POPSIZE;i++,j++)
	{
		if(flag)
		{
			if(s->fitness < nextgen[j]->fitness)
			{
				flag = 0;
				population[i] = s;
				i++;
			}
		}
		population[i] = nextgen[j];
	}
}


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
	Population aux;
	if(init)
	{
		init=0;
		l1 = malloc(2*sizeof(struct pl));

		l = malloc(sizeof(struct pl));
		l->info = malloc((POPSIZE+NEXTGENSIZE+NPOPS)*sizeof(Individual*));
	}
	
	l1[0].info = population;
	l1[0].size = size;
	l1[1].info = nextgen;
	l1[1].size = NEXTGENSIZE;

	l = mergepop(&l1[0],&l1[1]);
	aux = population;
	population = l->info;
	l->info = aux;


}


//elimina piores indivíduos entre pais e filhos
void bestreinsertion(Population nextgen)
{
	reins(nextgen,POPSIZE);
}


//mantém os melhores pais
void elitism(Population nextgen)
{
	int elite = POPSIZE - NEXTGENSIZE;
	int i;
	for(i=elite;i<POPSIZE;i++)
	{
		free(population[i]);
		population[i] = NULL;
	}
	reins(nextgen,elite);
}