//arquivo para teste de componentes etc


#include "genalg/genalg.h"


void initialize()
{
	int seed = time(NULL);
	srand(seed);
	getinput();
	initrecyclelist();
	init = 1;
}


void testconvergence2()
{
	int i,count,best=9999999;
	Individual* ind;
	initialize();

	for(i=0,count=0;i<1000;i++)
	{
		ind = genalg();
		if(ind->fitness<best)
		{
			best = ind->fitness;
			count = 1;
		}
		else if(ind->fitness==best)
		{
			count++;
		}
		recyclepopulation();
		free(population);
	}
	printf("Melhor fitness encontrado: %d\nConvergência: %d/%d\n",best,count,i);
}


void testconvergence()
{
	int i,count;
	Individual* ind;
	initialize();

	for(i=0,count=0;i<1000;i++)
	{
		ind = genalg();
		if(ind->fitness==44)
			count++;
		recyclepopulation();
		free(population);
	}
	printf("Convergência: %d/%d\n",count,i);
}


void testgenalg()
{
	int i,j;
	Individual* ind;
	initialize();
	genalg();
	printf("Final population:\n");
	for(i=0;i<POPSIZE;i++)
	{
		ind = population[i];
		printf("Individual %d\nTraits: ",i);
		for(j=0;j<grafo.n;j++)
		{
			printf("%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
		}
		printf("\nFitness: %d\n\n",ind->fitness);
	}
}


void testindividual3()
{
	initialize();
	int tr[2][18] = {{0,5,2,3,6,7,10,8,11,1,12,4,9,13,15,14,17,16},{1,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1}};
	Individual* ind = (Individual*) malloc(sizeof(Individual));
	ind->traits[0] = tr[0];
	ind->traits[1] = tr[1];

	printf("Fitness: %d\n",evaluate(ind));
}


void testreinsertion()
{
	initialize();
	int i,j;
	initpopulation();
	Individual* ind;

	printf("Initial Population:\n");
	for(i=0;i<POPSIZE;i++)
	{
		ind = population[i];
		printf("Individual %d\nTraits: ",i);
		for(j=0;j<grafo.n;j++)
		{
			printf("%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
		}
		printf("\nFitness: %d\n\n",ind->fitness);
	}

	nextgeneration();

	printf("\nChildren:\n\n");
	for(;i<POPSIZE+NEXTGENSIZE;i++)
	{
		ind = population[i];
		printf("Individual %d\nTraits: ",i);
		for(j=0;j<grafo.n;j++)
		{
			printf("%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
		}
		printf("\nFitness: %d\n\n",ind->fitness);
	}

	reinsertion(&population[POPSIZE]);

	printf("\nNew population:\n\n");
	for(i=0;i<POPSIZE+NEXTGENSIZE;i++)
	{
		ind = population[i];
		printf("Individual %d\nTraits: ",i);
		for(j=0;j<grafo.n;j++)
		{
			printf("%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
		}
		printf("\nFitness: %d\n\n",ind->fitness);
	}
}


void testnextgen()
{
	initialize();
	int i,j;
	initpopulation();
	Individual* ind;

	printf("Initial Population:\n");
	for(i=0;i<POPSIZE;i++)
	{
		ind = population[i];
		printf("Individual %d\nTraits: ",i);
		for(j=0;j<grafo.n;j++)
		{
			printf("%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
		}
		printf("\nFitness: %d\n\n",ind->fitness);
	}

	nextgeneration();

	printf("\nChildren:\n\n");
	for(;i<POPSIZE+NEXTGENSIZE;i++)
	{
		ind = population[i];
		printf("Individual %d\nTraits: ",i);
		for(j=0;j<grafo.n;j++)
		{
			printf("%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
		}
		printf("\nFitness: %d\n\n",ind->fitness);
	}
}


void testcrossover1()
{
	int i;
	getgraph("gauss18.txt");
	Individual* ind = newindividual();
	Individual* ind2 = newindividual();

	printf("parent1:\n");
	for(i=0;i<grafo.n;i++)
		printf("%d/%d,",ind->traits[0][i],ind->traits[1][i]);
	printf("\nFitness: %d\n",evaluate(ind));

	printf("parent2:\n");
	for(i=0;i<grafo.n;i++)
		printf("%d/%d,",ind2->traits[0][i],ind2->traits[1][i]);
	printf("\nFitness: %d\n",evaluate(ind2));

	ind = cyclecrossover(ind,ind2);
	ind2 = &ind[1];

	printf("child1:\n");
	for(i=0;i<grafo.n;i++)
		printf("%d/%d,",ind->traits[0][i],ind->traits[1][i]);
	printf("\nFitness: %d\n",evaluate(ind));

	printf("child2:\n");
	for(i=0;i<grafo.n;i++)
		printf("%d/%d,",ind2->traits[0][i],ind2->traits[1][i]);
	printf("\nFitness: %d\n",evaluate(ind2));
}


void testmutation1()
{
	int i;
	getgraph("grafoteste.txt");
	Individual* ind = newindividual();

	printf("individual:\n");
	for(i=0;i<grafo.n;i++)
		printf("%d/%d,",ind->traits[0][i],ind->traits[1][i]);
	printf("\n");

	mutation(ind);

	printf("\nmutated individual:\n");
	for(i=0;i<grafo.n;i++)
		printf("%d/%d,",ind->traits[0][i],ind->traits[1][i]);
	printf("\n");
}


void testinitpopulation1()
{
	int i,j;
	getgraph("gauss18.txt");
	POPSIZE = 50;
	NEXTGENSIZE = 30;
	initpopulation();
	Individual* ind;

	printf("Initial Population:\n");
	for(i=0;i<POPSIZE;i++)
	{
		ind = population[i];
		printf("Individual %d\nTraits: ",i);
		for(j=0;j<grafo.n;j++)
		{
			printf("%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
		}
		printf("\nFitness: %d\n\n",ind->fitness);
	}
}


//testa a função fitness com indivíduo aleatório para grafoteste.txt
void testfitness2()
{
	int i;
	getgraph("grafoteste.txt");
	Individual* ind = newindividual();

	printf("tasks: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[0][i]);
	printf("\nprocs: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[1][i]);
	printf("\n");
	printf("\nFitness: %d\n",evaluate(ind));
}


//testa a função fitness com solução ótima para o gauss18
void testfitness1()
{
	int i;
	getgraph("gauss18.txt");
	Individual* ind = newindividual();

	int otima[2][18] = {{0,2,6,3,5,8,1,11,4,10,7,9,13,15,16,12,14,17},{1,1,1,1,0,1,0,1,1,0,0,1,1,1,1,0,1,1}};

	for(i=0;i<grafo.n;i++)
	{
		ind->traits[0][i] = otima[0][i];
		ind->traits[1][i] = otima[1][i];
	}

	printf("tasks: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[0][i]);
	printf("\nprocs: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[1][i]);
	printf("\n");
	printf("\nFitness: %d\n",evaluate(ind));
}


//gera um indivíduo válido e exibe sua aptidão
void testindividual2()
{
	int i;
	getgraph("gauss18.txt");
	Individual* ind = newindividual();

	printf("tasks: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[0][i]);
	printf("\nprocs: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[1][i]);
	printf("\nFitness: %d\n",evaluate(ind));
}


//gera um indivíduo válido
void testindividual1()
{
	int i;
	getgraph("gauss18.txt");
	Individual* ind = newindividual();

	printf("tasks: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[0][i]);
	printf("\nprocs: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[1][i]);
	printf("\n");
}


//leitura de grafo de um arquivo e inicialização da estrutura
void testgraph1()
{
	int i;
	getgraph("gauss18.txt");
	Edge e;
	Node* no;

	for(i=0;i<grafo.n;i++)
	{
		no = &grafo.nodes[i];
		printf("Nó %d\n\tSucessores: ",no->id);
		for(e=no->successors; e!=NULL; e=e->next)
		{
			printf("%d, ",e->node->id);
		}
		printf("\n\tAntecessores: ");
		for(e=no->predecessors; e!=NULL; e=e->next)
		{
			printf("%d, ",e->node->id);
		}
		printf("\n\tNumero de antecessores: %d\n",no->predqty);
	}
}


int main()
{
	testconvergence2();
	return 0;
}