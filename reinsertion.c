#include "genalg.h"


//une duas listas ordenadas
poplist mergepop(poplist a,poplist b)
{
	int i,j,s=POPSIZE+NEXTGENSIZE;
	poplist l = malloc(sizeof(struct pl));
	l->info = malloc(s*sizeof(Individual*));

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


void reins(Population nextgen,int size)
{
	int i;
	poplist l = malloc(2*sizeof(struct pl));
	l[0].info = population;
	l[0].size = size;
	l[1].info = nextgen;
	l[1].size = NEXTGENSIZE;

	l = mergepop(&l[0],&l[1]);
	population = l->info;
}


//elimina piores indivíduos entre pais e filhos
void bestreinsertion(Population nextgen)
{
	reins(nextgen,POPSIZE);
}