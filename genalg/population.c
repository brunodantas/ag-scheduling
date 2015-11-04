//operações na população


#include "../genalg/genalg.h"


static inline void populationinsert(Population population,Individual *ind,int size);


//gera população inicial
Population* initpopulation()
{
	Individual* ind;
	population = malloc((POPSIZE+NEXTGENSIZE)*sizeof(Individual*));
	int i,j;
	for(i=0;i<POPSIZE;++i)
	{
		ind = newindividual();
		evaluate(ind);
		populationinsert(population,ind,i);
	}
	for(;i<POPSIZE+NEXTGENSIZE;i++)
		population[i] = NULL;

	return &population;
}


//insere indivíduo ordenado pela aptidão
static inline void populationinsert(Population population,Individual *ind,int size)
{
	int pos,i;
	for(pos=0;pos<size;++pos)
	{
		if(population[pos]->fitness > ind->fitness)
			break;
	}
	for(i=size;i>pos;i--)
		population[i] = population[i-1];

	population[pos] = ind;
}


//retorna um vetor de indivíduos descendentes da população atual
Population nextgeneration()
{
	int couples = 0;
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2,*a;
	int flag,i,r;

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = selection();
		p2 = selection();
		ind = crossover(p1,p2);
		ind2 = c[1];

		//mutação
		r = rand()%100;
		best();
		if(evaluate(ind)>=bestindividual->fitness && r<MUTATIONRATE)
		{
			r = rand()%2;
			if(r)
				mutation(ind);
			else
				mutation2(ind);
			evaluate(ind);
		}
		r = rand()%100;
		if(evaluate(ind2)>=bestindividual->fitness && r<MUTATIONRATE)
		{
			r = rand()%2;
			if(r)
				mutation(ind2);
			else
				mutation2(ind2);
			evaluate(ind2);
		}



		populationinsert(nextgen,ind,i);
		populationinsert(nextgen,ind2,i+1);
	}
	return nextgen;
}


//encontra o melhor individuo da população
void best()
{
	bestindividual = population[0];
}