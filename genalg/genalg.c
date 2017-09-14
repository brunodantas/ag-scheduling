#include "genalg.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>


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
		migration(i);
	}

	best();
	return bestindividual;
}


void runGA(int argc,char* argv[])
{
	int i,j,seed,a;
	int fd;
	double exptime,t1,t2;
	
	selection = &random_selection;
	if(argc>2)
	{
		seed = atoi(argv[1]);
		getgraph(argv[2]);
		POPSIZE = atoi(argv[3]);
		MAXGENERATIONS = atoi(argv[4]);
		NEXTGENSIZE = atoi(argv[5]);
		MUTATIONRATE = atoi(argv[6]);
		tournamentsize = atoi(argv[7]);
		PROCESSORQTY = atoi(argv[8]);
		MIGRATIONFREQ = atoi(argv[9]);
		MIGRATIONRATE = atoi(argv[10]);
		crossover = &cyclecrossover;
		reinsertion = &bestreinsertion;
	}
	else
	{
		seed = time(NULL);
		getinput();
	}

	init_mpop(); //sets subpopulations

	srand(seed + MYRANK);
	init = 1;

	if(MYRANK == 0)
	{
		t1 = MPI_Wtime();
	}

	genalg();
	Individual* ind = best_found();

	if(MYRANK != 0)
		return;

	t2 = MPI_Wtime();
	exptime = t2-t1;

	if(argc>1)
	{
		printf("%d, %f",ind->fitness,exptime);
	}
	else
	{
		printf("execution time: %f s\nbest fitness: %d\ncheck output.txt\n",exptime,ind->fitness);
		FILE *fp = fopen("output.txt", "ab+");
		fp = freopen("output.txt", "w",stdout);
		fprintf(fp,"Final population:\n");

		for(i=0;i<POPSIZE;i++)
		{
			ind = population[i];
			fprintf(fp,"Individual %d\n\tTraits:\n\t\t",i);
			for(j=0;j<grafo.n;j++)
			{
				// fprintf(fp,"%d/%d, ",ind->sequence[j],ind->processors[j]);
				fprintf(fp,"%d\t",ind->sequence[j]);
			}
			fprintf(fp,"\n\t\t");
			for(j=0;j<grafo.n;j++)
			{
				// fprintf(fp,"%d/%d, ",ind->sequence[j],ind->processors[j]);
				fprintf(fp,"%d\t",ind->processors[j]);
			}
			fprintf(fp,"\n\tFitness: %d\n",ind->fitness);
		}
	}
}


int main(int argc,char* argv[])
{
	MPI_Init(&argc, &argv);
	runGA(argc,argv);
	MPI_Finalize();
	return 0;
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
// 		printf("%d/%d,",ind->sequence[i],ind->processors[i]);
// 	printf("\nFitness: %d\n",ind->fitness);
// }