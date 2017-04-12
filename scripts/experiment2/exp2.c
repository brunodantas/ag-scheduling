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


//parâmetros obtidos no experiment1
#define MINPROCCESSORS 2
#define PROCESSORQTY 16
#define POPSIZE 200
#define MAXGENERATIONS 500
#define tournamentsize 2
#define CROSSOVERPERCENT 100
#define MUTATIONRATE 90

int threadqty = 4;
int experiments = 100;
int best[10];
int worst[10];
int fitnessacc[10];
int convergence[10];
int threadindex[10];
int NEXTGENSIZE;
char problema[50];
char output[10][20] = {"out1","out2","out3","out4","out5","out6","out7","out8","out9","out10"};
int seed[10];
int globalseed;
char* prob;
char problems[6][20] = {"GAUSS12","GAUSS18","GAUSS27","random30","random40","random50"};
int pqty = 6;
double timeacc[10];
int proc;


void *genalg(void *command)
{
	int a = system((char*) command);
	return NULL;
}


//função executada por cada thread
void *runAG(void *in)
{
	int i,a,fitness,p;
	int index = *(int *)in; //index da thread
	seed[index] = globalseed+(index*experiments);
	char* out = output[index];
	best[index] = 9999999;
	worst[index] = 0;
	fitnessacc[index] = 0;
	timeacc[index] = 0;
	char command[100];
	FILE *f;
	int texp = experiments/threadqty;
	char buf[32];
	double tim;
	pthread_t t;

	if(index == threadqty-1)
		texp += experiments%threadqty;

	for(i=0,convergence[index]=0;i<texp;i++,seed[index]++)
	{
		snprintf(command,100,"../../genalg/genalg %d %s %d %d %d %d %d %s %d",
			seed[index],problema,POPSIZE,MAXGENERATIONS,NEXTGENSIZE,MUTATIONRATE,tournamentsize,out,proc);
		//printf("%s\n",command);

		pthread_create(&t, NULL, genalg, command);

		int fd;
		while((fd = open(out, O_RDONLY))==-1);
		a = read(fd, buf, 32);
		close(fd);
		sscanf(buf,"%d, %lf",&fitness,&tim);
		// fitness = atoi(buf);

		fitnessacc[index] += fitness;
		timeacc[index] += tim;
		// printf("%s\n",buf);

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
}


void testconvergence()
{
	int i,a;
	struct timeval tim;
	// double exptime,t1,t2;
	pthread_t threads[10];

	// gettimeofday(&tim, NULL);  
	// t1=tim.tv_sec+(tim.tv_usec/1000000.0); 

	//printf("%s\n",problema);

	for (i = 0; i < threadqty-1; ++i)
	{
		threadindex[i] = i;
		pthread_create(&threads[i], NULL, runAG, &threadindex[i]);
	}

	runAG(&i);

	int b = best[threadqty-1];
	int c = convergence[threadqty-1];
	int w = worst[threadqty-1];
	int facc = fitnessacc[threadqty-1];
	double tacc = timeacc[threadqty-1];

	for (i = 0; i < threadqty-1; ++i)
	{
		pthread_join(threads[i], NULL);

		facc += fitnessacc[i];
		tacc += timeacc[i];
		
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

	double m = ((double)facc)/((double)experiments);
	double mt = tacc / ((double)experiments);
	double m2 = 100*(1-((double)b)/m);
	double w2 = 100*(1-((double)b)/((double)w));

	// gettimeofday(&tim, NULL); 
	// t2=tim.tv_sec+(tim.tv_usec/1000000.0); 
	// exptime = t2-t1;

	printf("%s\t%d\t%d\t%.1f (%.2f%%)\t%d (%.2f%%)\t%.3lf\t%d\n",prob,b,c,m,m2,w,w2,mt,proc);
}


void experiment2()
{
	int i;
	globalseed = time(NULL);
	NEXTGENSIZE = (CROSSOVERPERCENT * POPSIZE) / 100;
	// for(prob = 'f';prob <= 'j';prob++)
	// {
	// 	snprintf(problema,50,"../../problems/%c.txt",prob);
	// 	testconvergence();
	// }
	for (proc=MINPROCCESSORS; proc<=PROCESSORQTY; proc+=2)
	{
		for(i=0;i<pqty;i++)
		{
			prob = problems[i];
			snprintf(problema,50,"../../problems/%s.txt",prob);
			testconvergence();
		}
	}
}


int main(int argc,char* argv[])
{
	experiment2();
	return 0;
}