//operações na população


#include "../genalg/genalg.h"


static inline void populationinsert(Population population,Individual *ind,int size);


int compareind(const void * a,const void * b)
{
	Individual** ind = (Individual**) a;
	Individual** ind2 = (Individual**) b;
	return (*ind)->fitness - (*ind2)->fitness;
}


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

	qsort(population,POPSIZE,sizeof(Individual*),compareind);
	return &population;
}


//insere indivíduo ordenado pela aptidão
static inline void populationinsert(Population pop,Individual *ind,int size)
{
	pop[size] = ind;
}


//retorna um vetor de indivíduos descendentes da população atual
Population nextgeneration()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int flag,i,j,r;
	bestindividual = population[0];

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = selection();
		p2 = selection();
		ind = crossover(p1,p2);
		ind2 = c[1];

		//garantia de variedade genética
		if(evaluate(ind)>=bestindividual->fitness)
		{
			r = rand()%2;
			if(r)
				mutation(ind);
			else
				mutation2(ind);
		}
		if(evaluate(ind2)>=bestindividual->fitness)
		{
			r = rand()%2;
			if(r)
				mutation(ind2);
			else
				mutation2(ind2);
		}
		
		//mutação
		r = rand()%100;
		if(evaluate(ind)>=bestindividual->fitness && r<MUTATIONRATE)
		{
			r = rand()%2;
			if(r)
				mutation(ind);
			else
				mutation2(ind);
			evaluate(ind);
		}
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if(i+1 < NEXTGENSIZE && evaluate(ind2)>=bestindividual->fitness && r<MUTATIONRATE)
		{
			r = rand()%2;
			if(r)
				mutation(ind2);
			else
				mutation2(ind2);
			evaluate(ind2);
		}
		populationinsert(nextgen,ind2,i+1);
		
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//encontra o melhor individuo da população
void best()
{
	bestindividual = population[0];
}