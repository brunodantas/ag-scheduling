//arquivo para teste de componentes etc


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int experiments;
int best[4];
int convergence[4];
int threadindex[4];
int POPSIZE,NEXTGENSIZE,MAXGENERATIONS,MUTATIONRATE,tournamentsize;
char problema[50];
char output[4][20] = {"out1","out2","out3","out4"};
int seed[4];


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
	char command[100];
	FILE *f;
	int texp = experiments/4;
	char buf[10];
	pthread_t t;

	if(index == 3)
		texp += experiments%4;

	for(i=0,convergence[index]=0;i<texp;i++,seed[index]++)
	{
		snprintf(command,100,"./genalg/genalg %d %s %d %d %d %d %d %s",
			seed[index],problema,POPSIZE,MAXGENERATIONS,NEXTGENSIZE,MUTATIONRATE,tournamentsize,out);
		//printf("%s\n",command);

		pthread_create(&t, NULL, genalg, command);

		int fd;
		while((fd = open(out, O_RDONLY))==-1);
		a = read(fd, buf, 10);
		close(fd);
		fitness = atoi(buf);

		pthread_join(t, NULL);

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


void testconvergence2(int argc,char* argv[])
{
	int i,a;
	pthread_t threads[3];
	if(argc>1)
		experiments = atoi(argv[1]);
	else
		experiments = 100;

	getinput();
	printf("%s\n",problema);

	for (i = 0; i < 3; ++i)
	{
		threadindex[i] = i;
		pthread_create(&threads[i], NULL, runAG, &threadindex[i]);
	}

	runAG(&i);

	for (i = 0; i < 3; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	int b = best[0];
	int c = convergence[0];

	for(i=1;i<4;i++)
	{
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
	printf("Melhor fitness encontrado: %d\nConvergência: %d/%d\n",b,c,experiments);
}


void testsinglethread(char* argv[])
{
	int a,i,count,best=9999999,fitness;
	getinput();
	char command[80];
	char *out = output[0];
	FILE* f;
	experiments = atoi(argv[1]);
	int seed = time(NULL);

	for(i=0,count=0;i<experiments;i++,seed++)
	{
		snprintf(command,80,"./genalg/genalg %d %s %d %d %d %d %d %s",
			seed,problema,POPSIZE,MAXGENERATIONS,NEXTGENSIZE,MUTATIONRATE,tournamentsize,out);

		a = system(command);
		f = fopen(out,"r");
		a = fscanf(f,"%d",&fitness);

		if(fitness<best)
		{
			best = fitness;
			count = 1;
		}
		else if(fitness==best)
		{
			count++;
		}
		fclose(f);
	}
	printf("Melhor fitness encontrado: %d\nConvergência: %d/%d\n",best,count,i);
}


int main(int argc,char* argv[])
{
	testconvergence2(argc,argv);
	return 0;
}