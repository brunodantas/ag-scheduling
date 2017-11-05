//operações na população


#include "../genalg/genalg.h"


static inline void populationinsert(Population population,Individual *ind,int size);

Individual* (*selec)(void);
Individual* (*cross[10])(Individual*,Individual*);
void (*mut[2])(Individual*);
int ncross;
int nmut;


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
	population = malloc((POPSIZE+NEXTGENSIZE+NPOPS)*sizeof(Individual*));
	int i;
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


static inline void populationinsert(Population pop,Individual *ind,int size)
{
	pop[size] = ind;
}


Population run_config()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r=0,m=0;

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = selec();
		p2 = p1;
		while(p2 == p1)
			p2 = selec();
		if(ncross > 1)
			r = rand()%ncross;
		else
			r = 0;
		ind = cross[r](p1,p2);
		ind2 = c[1];

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			if(nmut > 1)
				m = rand()%2;
			else
				m = 0;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			if(nmut > 1)
				m = rand()%2;
			else
				m = 0;
			mut[m](ind2);
		}
		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


void init_config(int conf)
{
	selec = &tournament;
	mut[0] = &mutation_proc;
	mut[1] = &mutation_swap;
	nmut = 2;
	switch(conf)
	{
		case 0: //cycle crossover carry
			cross[0] = &cycle_crossover_carry;
			ncross = 1;
			break;

		case 1:	//omara config
			cross[0] = &one_point_seq_crossover;
			cross[1] = &one_point_proc_crossover;
			ncross = 2;
			nmut = 1;
			break;

		case 2: //omara config + seq_mutation
			cross[0] = &one_point_seq_crossover;
			cross[1] = &one_point_proc_crossover;
			ncross = 2;
			break;

		case 3: //correa config roullete
			selec = &roullete;
			cross[0] = &uniform_crossover_carry;
			ncross = 1;
			break;

		case 4: //correa config tournament
			cross[0] = &uniform_crossover_carry;
			ncross = 1;
			break;

		case 5: //config 2 for two point crossover
			cross[0] = &two_point_seq_crossover;
			cross[1] = &two_point_proc_crossover;
			ncross = 2;
			break;

		case 6: //one-point "both" crossover
			cross[0] = &one_point_both_crossover;
			ncross = 1;
			break;

		case 7: //uniform both crossover
			cross[0] = &uniform_both_crossover;
			ncross = 1;
			break;

		case 8: //two-point "both" crossover
			cross[0] = &two_point_both_crossover;
			ncross = 1;
			break;

		case 9: //config 2 for uniform crossover
			cross[0] = &uniform_seq_crossover;
			cross[1] = &uniform_proc_crossover;
			ncross = 2;
			break;

		case 10: //pmx carry
			cross[0] = &pmx_carry;
			ncross = 1;
			break;

		case 11: //ox carry
			cross[0] = &ox_carry;
			ncross = 1;
			break;

		case 12: //pmx + 2-point
			cross[0] = &pmx_two_point;
			ncross = 1;
			break;

		case 13: //cycle + 2-point
			cross[0] = &cycle_two_point;
			ncross = 1;
			break;

		case 14: //ox + 2-point
			cross[0] = &ox_two_point;
			ncross = 1;
			break;
	}
}


//encontra o melhor individuo da população
void best()
{
	bestindividual = population[0];
}


void mutate(Individual* ind)
{
	int r;
	int f = evaluate(ind);
	if(f >= bestindividual->fitness)
	{
		r = rand()%2;
		if(r)
			mutation(ind);
		else
			mutation_proc(ind);
	}
}


//old config
Population config_old()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r;
	bestindividual = population[0];
	reinsertion = &bestreinsertion;

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		ind = cycle_crossover_carry(p1,p2);
		ind2 = c[1];

		//garantia de variedade genética
		mutate(ind);
		mutate(ind2);
		
		//mutação
		r = rand()%100;
		if (r < MUTATIONRATE)
			mutate(ind);

		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
			mutate(ind2);

		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
		
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}