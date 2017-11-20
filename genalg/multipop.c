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
	if(NPOPS == 1 || MIGRATIONRATE == 0 || MIGRATIONFREQ == 0)
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
	// 			// fprintf(fp,"%d/%d, ",ind->sequence[j],ind->processors[j]);
	// 			printf("%d ",ind->sequence[j]);
	// 		}
	// 		printf("\n");
	// 		for(j=0;j<grafo.n;j++)
	// 		{
	// 			// fprintf(fp,"%d/%d, ",ind->sequence[j],ind->processors[j]);
	// 			printf("%d ",ind->processors[j]);
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
			migration_buffer[k] = population[i]->sequence[j];
			k++;
			migration_buffer[k] = population[i]->processors[j];
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
			population[i]->sequence[j] = migration_buffer[k];
			k++;
			population[i]->processors[j] = migration_buffer[k];
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
	// 			// fprintf(fp,"%d/%d, ",ind->sequence[j],ind->processors[j]);
	// 			printf("%d ",ind->sequence[j]);
	// 		}
	// 		printf("\n");
	// 		for(j=0;j<grafo.n;j++)
	// 		{
	// 			// fprintf(fp,"%d/%d, ",ind->sequence[j],ind->processors[j]);
	// 			printf("%d ",ind->processors[j]);
	// 		}
	// 		printf("\n\tFitness: %d\n",ind->fitness);
	// 	}
	// }

}


Individual* best_found()
{
	int i,j,k;
	if(migration == &do_nothing)
		return bestindividual;

	// serialize
	k=0;
	for(j=0;j<grafo.n;j++)
	{
		migration_buffer[k] = bestindividual->sequence[j];
		k++;
		migration_buffer[k] = bestindividual->processors[j];
		k++;
		// printf("%d/%d ",migration_buffer[k-2],migration_buffer[k-1]);
	}
	migration_buffer[k] = bestindividual->fitness;
	// printf("\n%d\n", migration_buffer[k-1]);

	MPI_Gather(migration_buffer,size_ind,MPI_INT,recv_buffer,size_ind,MPI_INT,0,MPI_COMM_WORLD);
	if(MYRANK!=0)
	{
		return bestindividual;
	}

	//unpack
	k=0;
	for(i=0;i<NPOPS;i++)
		population[i] = allocateindividual();
	for(i=0;i<NPOPS;i++)
	{
		for(j=0;j<grafo.n;j++)
		{
			population[i]->sequence[j] = recv_buffer[k];
			k++;
			population[i]->processors[j] = recv_buffer[k];
			k++;
		}
		population[i]->fitness = recv_buffer[k];
		k++;
	}

	qsort(population,NPOPS,sizeof(Individual*),compareind);

	bestindividual = population[0];
	return bestindividual;
}

