//arquivo para teste de componentes etc


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int threadqty;
int experiments;
int best[10];
int worst[10];
int convergence[10];
int threadindex[10];
int POPSIZE,NEXTGENSIZE,MAXGENERATIONS,MUTATIONRATE,tournamentsize;
char problema[50];
char output[10][20] = {"out1","out2","out3","out4","out5","out6","out7","out8","out9","out10"};
int seed[10];


void getinput()
{
	int i;
	FILE *f;

	f = fopen("input.txt","r");
	i = fscanf(f,"Problema: %s\n\n",problema);
	i = fscanf(f,"Population: %d\nGenerations: %d\nCrossover: %d%%\nMutation: %d%%\nTournament: %d\n",&POPSIZE,&MAXGENERATIONS,&NEXTGENSIZE,&MUTATIONRATE,&tournamentsize);
	NEXTGENSIZE *= POPSIZE;
	NEXTGENSIZE /= 100;
}


void *genalg(void *command)
{
	int a = system((char*) command);
	return NULL;
}


//função executada por cada thread
void *runAG(void *in)
{
	int i,a,fitness;
	int index = *(int *)in; //index da thread
	seed[index] = time(NULL)+(index*experiments);
	char* out = output[index];
	best[index] = 9999999;
	worst[index] = 0;
	char command[100];
	FILE *f;
	int texp = experiments/threadqty;
	char buf[10];
	pthread_t t;

	if(index == threadqty-1)
		texp += experiments%threadqty;

	for(i=0,convergence[index]=0;i<texp;i++,seed[index]++)
	{
		snprintf(command,100,"../../genalg/genalg %d %s %d %d %d %d %d %s",
			seed[index],problema,POPSIZE,MAXGENERATIONS,NEXTGENSIZE,MUTATIONRATE,tournamentsize,out);
		//printf("%s\n",command);

		pthread_create(&t, NULL, genalg, command);

		int fd;
		while((fd = open(out, O_RDONLY))==-1);
		a = read(fd, buf, 10);
		close(fd);
		fitness = atoi(buf);

		pthread_join(t, NULL);

		if(fitness>worst[index])
			worst[index] = fitness;
		if(fitness<best[index])
		{
			best[index] = fitness;
			convergence[index] = 1;
		}
		else if(fitness==best[index])
		{
			convergence[index]++;
		}
		//printf("%d\n",fitness);
	}
	snprintf(command,80,"rm %s",out);
	a = system(command);
	printf("thread: %d\texecutions: %d\tbest: %d\tconvergence: %d\n",index,texp,best[index],convergence[index]);
}


void testconvergence(int argc,char* argv[])
{
	int i,a;
	struct timeval tim;
	double exptime,t1,t2;
	pthread_t threads[10];

	gettimeofday(&tim, NULL);  
	t1=tim.tv_sec+(tim.tv_usec/1000000.0); 

	if(argc>1)
		experiments = atoi(argv[1]);
	else
		experiments = 100;

	if(argc>2)
		threadqty = atoi(argv[2]);
	else
		threadqty = 4;	

	getinput();
	printf("%s\n",problema);

	for (i = 0; i < threadqty-1; ++i)
	{
		threadindex[i] = i;
		pthread_create(&threads[i], NULL, runAG, &threadindex[i]);
	}

	runAG(&i);

	int b = best[threadqty-1];
	int c = convergence[threadqty-1];
	int w = worst[threadqty-1];

	for (i = 0; i < threadqty-1; ++i)
	{
		pthread_join(threads[i], NULL);
		
		if(worst[i] > w)
			w = worst[i];
		if(best[i]<b)
		{
			b = best[i];
			c = convergence[i];
		}
		else if(best[i]==b)
		{
			c += convergence[i];
		}
		//printf("%d\n",fitness);
	}

	gettimeofday(&tim, NULL); 
	t2=tim.tv_sec+(tim.tv_usec/1000000.0); 
	exptime = t2-t1;

	printf("Execution time: %f s\nBest fitness: %d\nConvergence: %d/%d\nWorst fitness: %d\n",exptime,b,c,experiments,w);
}


int main(int argc,char* argv[])
{
	testconvergence(argc,argv);
	return 0;
}