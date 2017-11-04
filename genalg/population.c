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
Population config0()
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


//omara config
Population config1()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r;
	Individual* (*cross[2])(Individual*,Individual*) = 
				{&one_point_seq_crossover,&one_point_proc_crossover};

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		r = rand()%2;
		ind = cross[r](p1,p2);
		ind2 = c[1];
		
		//mutação
		r = rand()%100;
		if (r < MUTATIONRATE)
			mutation_proc(ind);
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
			mutation_proc(ind2);
		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//omara config + seq_mutation
Population config2()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r,m;
	Individual* (*cross[2])(Individual*,Individual*) = 
				{&one_point_seq_crossover,&one_point_proc_crossover};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		r = rand()%2;
		ind = cross[r](p1,p2);
		ind2 = c[1];
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind2);
		}
		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//correa config roullete
Population config3()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r,m;
	Individual* (*cross[2])(Individual*,Individual*) = 
				{&uniform_seq_crossover,&uniform_proc_crossover};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = roullete();
		p2 = p1;
		while(p2 == p1)
			p2 = roullete();
		r = rand()%2;
		ind = cross[r](p1,p2);
		ind2 = c[1];
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind2);
		}
		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//correa config tournament
Population config4()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r,m;
	Individual* (*cross[2])(Individual*,Individual*) = 
				{&uniform_seq_crossover,&uniform_proc_crossover};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		r = rand()%2;
		ind = cross[r](p1,p2);
		ind2 = c[1];
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind2);
		}
		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//two-point crossovers
Population config5()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r,m;
	Individual* (*cross[2])(Individual*,Individual*) = 
				{&two_point_seq_crossover,&two_point_proc_crossover};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		r = rand()%2;
		ind = cross[r](p1,p2);
		ind2 = c[1];
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind2);
		}
		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//one-point both crossover
Population config6()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r,m;
	Individual* (*cross[1])(Individual*,Individual*) = 
				{&one_point_both_crossover};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		ind = cross[0](p1,p2);
		ind2 = c[1];
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind2);
		}
		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//omara config + seq_mutation, single child
Population config7()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind;
	int i,r,m;
	Individual* (*cross[2])(Individual*,Individual*) = 
				{&one_point_seq_crossover,&one_point_proc_crossover};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i++)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		r = rand()%2;
		ind = cross[r](p1,p2);
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//uniform both crossover
Population config8()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r,m;
	Individual* (*cross[1])(Individual*,Individual*) = 
				{&uniform_both_crossover};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		ind = cross[0](p1,p2);
		ind2 = c[1];
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind2);
		}
		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//ox seq
Population config9()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r,m;
	Individual* (*cross[1])(Individual*,Individual*) = 
				{&ox_seq};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		ind = cross[0](p1,p2);
		ind2 = c[1];
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind2);
		}
		evaluate(ind2);
		populationinsert(nextgen,ind2,i+1);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//ox seq + one-point proc
Population config10()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind;
	int i,r,m;
	Individual* (*cross[2])(Individual*,Individual*) = 
				{&ox_seq,&one_point_proc_crossover};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i++)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		r = rand()%2;
		ind = cross[r](p1,p2);
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);
	}
	qsort(nextgen,NEXTGENSIZE,sizeof(Individual*),compareind);
	return nextgen;
}


//ox carry
Population config11()
{
	Population nextgen = &population[POPSIZE];
	Individual *p1,*p2,*ind,*ind2;
	int i,r,m;
	Individual* (*cross[1])(Individual*,Individual*) = 
				{&ox_carry};

	void (*mut[2])(Individual*) = 
				{&mutation_swap,&mutation_proc};

	for(i=0;i<NEXTGENSIZE;i+=2)
	{
		//escolha dos pais e crossover
		p1 = tournament();
		p2 = p1;
		while(p2 == p1)
			p2 = tournament();
		ind = cross[0](p1,p2);
		ind2 = c[1];
		
		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind);
		}
		evaluate(ind);
		populationinsert(nextgen,ind,i);

		r = rand()%100;
		if (r < MUTATIONRATE)
		{
			m = rand()%2;
			mut[m](ind2);
		}
		evaluate(ind2);
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