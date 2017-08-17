//operações multipopulação


#include "genalg.h"


int size_ind;
int mbuffer_size;
int* migration_buffer;
int* recv_buffer;

void do_nothing(int x){}


void init_mpop()
{
	int i;
	MPI_Comm_size(MPI_COMM_WORLD, &NPOPS);
	MPI_Comm_rank(MPI_COMM_WORLD, &MYRANK);

	POPSIZE /= NPOPS;
	NEXTGENSIZE *= POPSIZE;
	NEXTGENSIZE /= 100;

	if(NPOPS == 1)
	{
		migration = &do_nothing;
		return;
	}

	// printf("%d\n", POPSIZE);
	MIGRATIONRATE = (MIGRATIONRATE * POPSIZE) / 100;
	if(MIGRATIONRATE == 0)	//prevent empty migration
		MIGRATIONRATE = 1;
	migration = &ring_migration;
	size_ind = 2*grafo.n + 1;
	mbuffer_size = MIGRATIONRATE * size_ind;

	migration_buffer = malloc(mbuffer_size*sizeof(int));
	recv_buffer = malloc(NPOPS*size_ind*sizeof(int));
}


void ring_migration(int generation)
{
	int i,j,k, source, destination;
	source = (MYRANK == 0)? NPOPS-1 : MYRANK-1;
	destination = (MYRANK+1) % NPOPS;

	if(generation == 0 || generation%MIGRATIONFREQ != 0)
		return;
	// printf("migr%d\n",generation);

	// if(MYRANK==0)
	// {
	// 	printf("\n\nMIGRATION BEGIN\n");
	// 	Individual* ind;
	// 	for(i=0;i<POPSIZE;i++)
	// 	{
	// 		ind = population[i];
	// 		printf("Individual %d\nTraits:\n",i);
	// 		for(j=0;j<grafo.n;j++)
	// 		{
	// 			// fprintf(fp,"%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
	// 			printf("%d ",ind->traits[0][j]);
	// 		}
	// 		printf("\n");
	// 		for(j=0;j<grafo.n;j++)
	// 		{
	// 			// fprintf(fp,"%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
	// 			printf("%d ",ind->traits[1][j]);
	// 		}
	// 		printf("\n\tFitness: %d\n",ind->fitness);
	// 	}
	// }

	//serialize
	k=0;
	for(i=0;i<MIGRATIONRATE;i++)
	{
		for(j=0;j<grafo.n;j++)
		{
			migration_buffer[k] = population[i]->traits[0][j];
			k++;
			migration_buffer[k] = population[i]->traits[1][j];
			k++;
			// printf("%d/%d ",migration_buffer[k-2],migration_buffer[k-1]);
		}
		migration_buffer[k] = population[i]->fitness;
		k++;
		// printf("\n%d\n", migration_buffer[k-1]);
	}

	MPI_Sendrecv_replace(migration_buffer, mbuffer_size, MPI_INT, destination, 0,
						source, 0, MPI_COMM_WORLD, NULL);
	//unpack
	k=0;
	for(i=0;i<MIGRATIONRATE;i++)
	{
		for(j=0;j<grafo.n;j++)
		{
			population[i]->traits[0][j] = migration_buffer[k];
			k++;
			population[i]->traits[1][j] = migration_buffer[k];
			k++;
		}
		population[i]->fitness = migration_buffer[k];
		k++;
	}
	qsort(population,POPSIZE,sizeof(Individual*),compareind);

	// if(MYRANK==0)
	// {
	// 	printf("\n\nMIGRATION IN\n");
	// 	Individual* ind;
	// 	for(i=0;i<POPSIZE;i++)
	// 	{
	// 		ind = population[i];
	// 		printf("Individual %d\nTraits:\n",i);
	// 		for(j=0;j<grafo.n;j++)
	// 		{
	// 			// fprintf(fp,"%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
	// 			printf("%d ",ind->traits[0][j]);
	// 		}
	// 		printf("\n");
	// 		for(j=0;j<grafo.n;j++)
	// 		{
	// 			// fprintf(fp,"%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
	// 			printf("%d ",ind->traits[1][j]);
	// 		}
	// 		printf("\n\tFitness: %d\n",ind->fitness);
	// 	}
	// }

}


Individual* best_found()
{
	int i,j,k;
	if(NPOPS==1)
		return bestindividual;

	// serialize
	k=0;
	for(j=0;j<grafo.n;j++)
	{
		migration_buffer[k] = bestindividual->traits[0][j];
		k++;
		migration_buffer[k] = bestindividual->traits[1][j];
		k++;
		// printf("%d/%d ",migration_buffer[k-2],migration_buffer[k-1]);
	}
	migration_buffer[k] = bestindividual->fitness;
	// k++;
	// printf("\n%d\n", migration_buffer[k-1]);

	MPI_Gather(migration_buffer,size_ind,MPI_INT,recv_buffer,size_ind,MPI_INT,0,MPI_COMM_WORLD);
	if(MYRANK!=0)
	{
		return bestindividual;
	}

	//unpack
	k=0;
	for(i=0;i<NPOPS;i++)
	{
		for(j=0;j<grafo.n;j++)
		{
			population[i]->traits[0][j] = recv_buffer[k];
			k++;
			population[i]->traits[1][j] = recv_buffer[k];
			k++;
		}
		population[i]->fitness = recv_buffer[k];
		k++;
	}

	qsort(population,NPOPS,sizeof(Individual*),compareind);

	bestindividual = population[0];
	return bestindividual;
}

