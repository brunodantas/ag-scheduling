//arquivo para teste de componentes etc


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

int MINPROCCESSOR;
int MAXPROCESSOR;
int POPSIZE;
int NEXTGENSIZE;
int MAXGENERATIONS;
int tournamentsize = 2;
int MUTATIONRATE;

int experiments;
int best;
int worst;
int fitnessacc;
int convergence;
char problema[50];
char output[10] = "out";
int seed;
char prob[20];
double timeacc;
int proc;


//leitura do arquivo input.txt
void getinput()
{
	int i;
	FILE *f;

	f = fopen("input.txt","r");
	i = fscanf(f,"Experiments: %d\nProcessors: %d-%d\nPopulation: %d\nGenerations: %d\nCrossover: %d%%\nMutation: %d%%\n",
		&experiments, &MINPROCCESSOR,&MAXPROCESSOR,&POPSIZE,&MAXGENERATIONS,&NEXTGENSIZE,&MUTATIONRATE);
	NEXTGENSIZE *= POPSIZE;
	NEXTGENSIZE /= 100;
	fclose(f);
}


void *genalg(void *command)
{
	int a = system((char*) command);
	return NULL;
}


void testconvergence()
{
	int i,a,fitness,p;
	char* out = output;
	best = 9999999;
	worst = 0;
	fitnessacc = 0;
	timeacc = 0;
	char command[100];
	FILE *f;
	char buf[32];
	double tim;
	pthread_t t;

	convergence=0;
	for(i=0;i<experiments;i++,seed++)
	{
		snprintf(command,100,"../genalg/genalg %d %s %d %d %d %d %d %s %d",
			seed,problema,POPSIZE,MAXGENERATIONS,NEXTGENSIZE,MUTATIONRATE,tournamentsize,out,proc);

		// printf("%s\n",command);
		pthread_create(&t, NULL, genalg, command);

		int fd;
		while((fd = open(out, O_RDONLY))==-1)
			usleep(10);
		pthread_join(t, NULL);
		a = read(fd, buf, 32);
		close(fd);
		sscanf(buf,"%d, %lf",&fitness,&tim);

		fitnessacc += fitness;
		timeacc += tim;

		if(fitness>worst)
			worst = fitness;
		if(fitness<best)
		{
			best = fitness;
			convergence = 1;
		}
		else if(fitness==best)
		{
			convergence++;
		}
	}
	snprintf(command,80,"rm %s",out);
	a = system(command);

	double m = ((double)fitnessacc)/((double)experiments);
	double mt = timeacc / ((double)experiments);
	double m2 = 100*(1-((double)best)/m);
	double w2 = 100*(1-((double)best)/((double)worst));

	// gettimeofday(&tim, NULL); 
	// t2=tim.tv_sec+(tim.tv_usec/1000000.0); 
	// exptime = t2-t1;
	sscanf(problema, "../problems/%s",prob);
	prob[strlen(prob)-4] = '\0';
	printf("%10s\t%d\t%d/%d\t%.1f (%.2f%%)\t%d (%.2f%%)\t%.3lf\t%d\n",prob,best,convergence,experiments,m,m2,worst,w2,mt,proc);
}


int main(int argc,char* argv[])
{
	int i;
	seed = time(NULL);
	getinput();
	printf("\npopulation = %d, generations = %d, crossovers = %d, mutation = %d%%\n\n",POPSIZE,MAXGENERATIONS,NEXTGENSIZE,MUTATIONRATE);
	printf("%10s\tbest\tconv\t%10s\t%10s\ttime\tprocs\n","grafo","avg","worst");
	for(i=1;i<argc;i++)
	{
		for (proc=MINPROCCESSOR; proc<=MAXPROCESSOR; proc+=2)
		{
			snprintf(problema,50,"%s",argv[i]);
			testconvergence();
		}
	}
	printf("\n");
	return 0;
}