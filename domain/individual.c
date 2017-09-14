//operações no indivíduo


#include "../genalg/genalg.h"


//gera um indivíduo válido
Individual* newindividual()
{
	int i,r,count=0,currenttaskid;
	Edge e;
	list availabletasks = newlist(grafo.n);
	int* predecessorsleft = malloc(grafo.n*sizeof(int));
	Individual* ind = allocateindividual();

	//inicializa e procura nós sem predecessores
	for(i=0;i<grafo.n;i++)
	{
		predecessorsleft[i] = grafo.nodes[i].predqty;
		if(predecessorsleft[i]==0)
		{
			add(availabletasks,i);
		}
	}

	while(availabletasks->size > 0)
	{
		r = rand() % availabletasks->size;
		currenttaskid = at(availabletasks,r);
		erase(availabletasks,r);

		ind->sequence[count] = currenttaskid;

		//if(count==0)
		//	ind->processors[count] = 0;	//obrigar primeiro task a ser alocado no primeiro processador
		//else
		ind->processors[count] = rand()%PROCESSORQTY;
		count++;

		//adiciona à lista nós cujos predecessores já foram escolhidos
		for(e = grafo.nodes[currenttaskid].successors; e!=NULL; e = e->next)
		{
			predecessorsleft[e->node->id]--;
			if(predecessorsleft[e->node->id]==0)
			{
				add(availabletasks,e->node->id);
			}
		}
	}

	free(availabletasks->info);
	free(availabletasks);
	free(predecessorsleft);
	return ind;
}


// static inline int max(int a,int b)
// {
// 	if(a>b)
// 		return a;
// 	return b;
// }


static int max(int* arr)
{
	int i,m = arr[0];
	for (i=1;i<PROCESSORQTY;i++)
	{
		if (arr[i] > m)
			m = arr[i];
	}
	return m;
}


#define TASK ind->sequence
#define PROCESSOR ind->processors


static void gettasktime(Individual *ind, int taskindex, int* totaltime, int* timestamp)
{
	int i,a;
	int max = totaltime[PROCESSOR[taskindex]];
	Edge e;

	for(i=0; i<taskindex; i++)
	{
		for(e = grafo.nodes[TASK[taskindex]].predecessors; e!=NULL; e=e->next)
		{
			if(e->node->id == TASK[i] && PROCESSOR[i] != PROCESSOR[taskindex]) //se é predecessor e foi executado em processador diferente
			{
				a = timestamp[TASK[i]] + e->node->cost + e->cost;
				if(a>max)
					max = a;
			}
		}
	}

	timestamp[TASK[taskindex]] = max;
	totaltime[PROCESSOR[taskindex]] = max + grafo.nodes[TASK[taskindex]].cost;
}


//calcula e seta aptidão do indivíduo
int evaluate(Individual *ind)
{
	int i;
	// int totaltime[2] = {0,0};
	int* totaltime;
	totaltime = (int*) malloc(PROCESSORQTY*sizeof(int));
	for (i=0;i<PROCESSORQTY;i++)
		totaltime[i] = 0;

	int* timestamps = (int*)  malloc(grafo.n*sizeof(int));
	totaltime[PROCESSOR[TASK[0]]] += grafo.nodes[TASK[0]].cost;
	timestamps[TASK[0]] = 0;

	for(i=1; i<grafo.n; i++)
	{
		gettasktime(ind, i, totaltime, timestamps);

		//printf("task: %2d timestamp: %d\n",TASK[i],timestamp[TASK[i]]);
	}
	
	// ind->fitness = max(totaltime[0],totaltime[1]);
	ind->fitness = max(totaltime);

	// for(i=0;i<grafo.n;i++)
	// {
	// 	printf("%d %d\n",TASK[i],timestamp[TASK[i]]);
	// }
	
	// printf("ASDFASDF\n");
	free(totaltime);
	return ind->fitness;
}


//troca dois genes de lugar
void mutation(Individual *ind)
{
	int flag = 0;
	int i;
	int* genes = (int*)  malloc(grafo.n*sizeof(int));
	char temp[2];
	int a,b;
	a = rand()%grafo.n;
	b = a;
	for(b=a;b==a;b = rand()%grafo.n);
	//printf("%d\t%d\n",a,b);

	for(i=0;i<grafo.n;i++)
		genes[i] = ind->sequence[i];

	temp[0] = ind->sequence[a];
	temp[1] = ind->processors[a];
	ind->sequence[a] = ind->sequence[b];
	ind->processors[a] = ind->processors[b];
	ind->sequence[b] = temp[0];
	ind->processors[b] = temp[1];

	makevalid(ind);

	for(i=0;i<grafo.n;i++)
	{
		if(genes[i] != ind->sequence[i])
		{
			free(genes);
			return;
		}
	}
	free(genes);
	mutation(ind);
}


//troca o processador de uma tarefa
void mutation2(Individual *ind)
{
	int a = rand()%grafo.n;
	int b = ind->processors[a];

	while (b == ind->processors[a])
		b = rand()%PROCESSORQTY;
	ind->processors[a] = b;
	// ind->processors[a] = !ind->processors[a];
}


//torna indivíduo válido
void makevalid(Individual *ind)
{
	int i,j;
	list genes[2];
	genes[0] = newlist(grafo.n);
	genes[1] = newlist(grafo.n);
	int* newgenes[2];
	newgenes[0] = (int*)  malloc(grafo.n*sizeof(int));
	newgenes[1] = (int*)  malloc(grafo.n*sizeof(int));

	int* predecessorsleft = (int*)  malloc(grafo.n*sizeof(int));
	Edge e;
	int currenttaskid;

	for(i=0; i<grafo.n; i++)
	{
		add(genes[0],ind->sequence[i]);
		add(genes[1],ind->processors[i]);
		predecessorsleft[i] = grafo.nodes[i].predqty;
	}

	for(i=0; i<grafo.n; i++)
	{
		for(j=0;;j++)
		{
			currenttaskid = at(genes[0],j);
			if(predecessorsleft[currenttaskid] == 0)
			{
				newgenes[0][i] = currenttaskid;
				newgenes[1][i] = at(genes[1],j);
				erase(genes[0],j);
				erase(genes[1],j);

				for(e = grafo.nodes[currenttaskid].successors; e!=NULL; e = e->next)
					predecessorsleft[e->node->id]--;

				break;
			}
		}
	}
	free(genes[0]->info);
	free(genes[1]->info);
	free(genes[0]);
	free(genes[1]);
	free(ind->sequence);
	free(ind->processors);
	ind->sequence = newgenes[0];
	ind->processors = newgenes[1];
}


Individual* allocateindividual()
{
	Individual* ind;
	ind = (Individual*) malloc(sizeof(Individual));
	ind->sequence = (int*)  malloc(grafo.n*sizeof(int));
	ind->processors = (int*)  malloc(grafo.n*sizeof(int));
	
	return ind;
}