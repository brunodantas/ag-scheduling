#include "genalg.h"


//algoritmo genético
Individual* genalg()
{
	Population nextgen;
	int i,j;

	initpopulation();
	best();

	//main loop
	for(i=0;i<MAXGENERATIONS;i++)
	{
		nextgen = nextgeneration();
		reinsertion(nextgen);
		best();
	}

	return bestindividual;
}


void runGA()
{
	int i;
	int seed = time(NULL);
	Individual* ind;
	srand(seed);
	getinput();
	ind = genalg();

	printf("best individual:\n");
	for(i=0;i<grafo.n;i++)
		printf("%d/%d,",ind->traits[0][i],ind->traits[1][i]);
	printf("\nFitness: %d\n",ind->fitness);
}


int main()
{
	runGA();
	return 0;
}