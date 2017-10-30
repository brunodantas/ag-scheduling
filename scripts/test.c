//arquivo para teste de componentes etc


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int MINPROCCESSOR;
int MAXPROCESSOR;
int POPSIZE;
int NEXTGENSIZE;
int MAXGENERATIONS;
int CONF;
int MUTATIONRATE;
int MIGRATIONFREQ;
int MIGRATIONRATE;
int NPOPS;

int experiments;
int best;
int worst;
int fitnessacc;
int convergence;
char problema[50];
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
	i = fscanf(f,"Experiments: %d\nProcessors: %d-%d\nPopulation: %d\nGenerations: %d\nCrossover: %d%%\nMutation: %d%%\nConfiguration: %d\n\nMigrationFreq: %d\nMigrationRate: %d%%\nPopulations: %d",
		&experiments, &MINPROCCESSOR,&MAXPROCESSOR,&POPSIZE,&MAXGENERATIONS,&NEXTGENSIZE,&MUTATIONRATE,&CONF,&MIGRATIONFREQ,&MIGRATIONRATE,&NPOPS);
	NEXTGENSIZE *= POPSIZE;
	NEXTGENSIZE /= 100;
	fclose(f);
}


void testconvergence()
{
	int i,a,fitness,p;
	best = 9999999;
	worst = 0;
	fitnessacc = 0;
	timeacc = 0;
	char command[100];
	FILE *f;
	// char buf[32];
	char* buf;
	double tim;
	size_t len = 32;

	convergence=0;
	for(i=0;i<experiments;i++,seed+=NPOPS)
	{
		snprintf(command,100,"mpiexec -n %d ../genalg/genalg %d %s %d %d %d %d %d %d %d %d",
			NPOPS,seed,problema,POPSIZE,MAXGENERATIONS,NEXTGENSIZE,MUTATIONRATE,CONF,proc,MIGRATIONFREQ,MIGRATIONRATE);

		// printf("%s\n",command);
		f = popen(command,"r");
		while (getline(&buf, &len, f) != -1);
		pclose(f);
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
	int i, nprobs = argc;
	char** probs = argv;
	if (argc < 2)
	{
		printf("usage: %s {list of problem files}\n",argv[0]);
		return 1;
	}
	seed = time(NULL);
	getinput();
	printf("\nexperiments: %d\nconfiguration = %d,population = %d, generations = %d, crossovers = %d, mutation = %d%%, subpopulations = %d, migrationfreq = %d, migrationrate = %d%%\n\n",experiments,CONF,POPSIZE,MAXGENERATIONS,NEXTGENSIZE,MUTATIONRATE,NPOPS,MIGRATIONFREQ,MIGRATIONRATE);
	printf("%10s\tbest\tconv\t%10s\t%10s\ttime\tprocs\n","grafo","avg","worst");
	for(i=1;i<nprobs;i++)
	{
		for (proc=MINPROCCESSOR; proc<=MAXPROCESSOR; proc+=2)
		{
			snprintf(problema,50,"%s",probs[i]);
			testconvergence();
		}
	}
	printf("\n");
	return 0;
}