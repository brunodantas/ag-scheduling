#include "genalg.h"


//algoritmo genético
Individual* genalg()
{
	Population nextgen;
	int i,j;

	initpopulation();

	//main loop
	for(i=0;i<MAXGENERATIONS;i++)
	{
		nextgen = nextgeneration();
		reinsertion(nextgen);
	}

	best();
	return bestindividual;
}


// void runGA()
// {
// 	int i;
// 	int seed = time(NULL);
// 	srand(seed);
// 	getinput();
// 	initrecyclelist();
// 	init = 1;

// 	printf("best individual:\n");
// 	for(i=0;i<grafo.n;i++)
// 		printf("%d/%d,",ind->traits[0][i],ind->traits[1][i]);
// 	printf("\nFitness: %d\n",ind->fitness);
// }