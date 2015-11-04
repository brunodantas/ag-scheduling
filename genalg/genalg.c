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
	}

	best();
	return bestindividual;
}


void runGA(int argc,char* argv[])
{
	int i,j,seed,a;
	char* output;
	char out[10];
	int fd;
	
	if(argc>2)
	{
		seed = atoi(argv[1]);
		getgraph(argv[2]);
		POPSIZE = atoi(argv[3]);
		MAXGENERATIONS = atoi(argv[4]);
		NEXTGENSIZE = atoi(argv[5]);
		MUTATIONRATE = atoi(argv[6]);
		tournamentsize = atoi(argv[7]);
		output = argv[8];
		selection = &tournament;
		crossover = &cyclecrossover;
		reinsertion = &elitism;
	}
	else
	{
		seed = time(NULL);
		getinput();
	}

	srand(seed);
	initrecyclelist();
	init = 1;

	Individual* ind = genalg();

	if(argc>1)
	{
		snprintf(out,10,"%d",ind->fitness);
		fd = mkfifo(output, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);;
		fd = open(output, O_WRONLY);
		a = write(fd, out, sizeof(out));
    	close(fd);
    	unlink(out);
	}
	else
	{
		printf("best fitness: %d\ncheck output.txt\n",ind->fitness);
		FILE *fp = fopen("output.txt", "ab+");
		fp = freopen("output.txt", "w",stdout);
		fprintf(fp,"Final population:\n");

		for(i=0;i<POPSIZE;i++)
		{
			ind = population[i];
			fprintf(fp,"Individual %d\n\tTraits: ",i);
			for(j=0;j<grafo.n;j++)
			{
				fprintf(fp,"%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
			}
			fprintf(fp,"\n\tFitness: %d\n",ind->fitness);
		}
	}
}


int main(int argc,char* argv[])
{
	runGA(argc,argv);

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
// 		printf("%d/%d,",ind->traits[0][i],ind->traits[1][i]);
// 	printf("\nFitness: %d\n",ind->fitness);
// }