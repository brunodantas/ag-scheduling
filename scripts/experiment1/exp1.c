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
int convergence[10];
int threadindex[10];
int POPSIZE,NEXTGENSIZE,MAXGENERATIONS,MUTATIONRATE,tournamentsize;
char problema[50] = "../../problems/gauss18.txt";
char output[10][20] = {"out1","out2","out3","out4","out5","out6","out7","out8","out9","out10"};
int seed[10];
int globalseed;


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
	seed[index] = globalseed+(index*experiments);
	char* out = output[index];
	best[index] = 9999999;
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
}


void testconvergence()
{
	int i,a;
	struct timeval tim;
	double exptime,t1,t2;
	pthread_t threads[10];

	gettimeofday(&tim, NULL);  
	t1=tim.tv_sec+(tim.tv_usec/1000000.0); 

	experiments = 100;
	threadqty = 4;	

	//printf("%s\n",problema);

	for (i = 0; i < threadqty-1; ++i)
	{
		threadindex[i] = i;
		pthread_create(&threads[i], NULL, runAG, &threadindex[i]);
	}

	runAG(&i);

	int b = best[threadqty-1];
	int c = convergence[threadqty-1];

	for (i = 0; i < threadqty-1; ++i)
	{
		pthread_join(threads[i], NULL);
		
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

	printf("P%dG%dT%d\t%d\n",POPSIZE,MAXGENERATIONS,tournamentsize,c);
}


void exp1()
{
	int i,j,k;
	MUTATIONRATE = 30;
	globalseed = time(NULL);

	for(POPSIZE = 50;POPSIZE <= 200; POPSIZE += POPSIZE)
	{
		NEXTGENSIZE = 60 * POPSIZE;
		NEXTGENSIZE /= 100;
		for(MAXGENERATIONS = 200;MAXGENERATIONS <= 500;MAXGENERATIONS += 100)
		{
			if(MAXGENERATIONS == 400)
				MAXGENERATIONS = 500;

			for(tournamentsize = 2; tournamentsize <= 4; tournamentsize++)
			{
				testconvergence();
				globalseed++;
			}
		}
	}

}


int main(int argc,char* argv[])
{
	exp1();
	return 0;
}