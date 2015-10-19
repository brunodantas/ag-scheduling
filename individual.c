//operações no indivíduo


#include "genalg.h"


//gera um indivíduo válido
Individual* newindividual()
{
	int i,r,count=0,currenttaskid;
	Edge e;
	list availabletasks = newlist(grafo.n);
	int* predecessorsleft = malloc(grafo.n*sizeof(int));
	Individual* ind = (Individual*) malloc(sizeof(Individual));
	ind->traits[0] = (int*)  malloc(grafo.n*sizeof(int));
	ind->traits[1] = (int*)  malloc(grafo.n*sizeof(int));

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

		ind->traits[0][count] = currenttaskid;

		//if(count==0)
		//	ind->traits[1][count] = 0;	//obrigar primeiro task a ser alocado no primeiro processador
		//else
		ind->traits[1][count] = rand()%2;
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
	return ind;
}


int max(int a,int b)
{
	if(a>b)
		return a;
	return b;
}


#define TASK ind->traits[0]
#define PROCESSOR ind->traits[1]


void gettasktime(Individual *ind, int taskindex, int* totaltime, int* timestamp)
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
	totaltime[PROCESSOR[taskindex]] = max + grafo.nodes[TASK[i]].cost;
}


//calcula e seta aptidão do indivíduo
int evaluate(Individual *ind)
{
	int i;
	int* timestamp = malloc(grafo.n*sizeof(int));
	int totaltime[2] = {0,0};
	int last[2] = {-1,-1};

	totaltime[PROCESSOR[0]] += grafo.nodes[TASK[0]].cost;
	timestamp[TASK[0]] = 0;
	last[PROCESSOR[0]] = TASK[0];

	for(i=1; i<grafo.n; i++)
	{
		if(PROCESSOR[i] == PROCESSOR[i-1]) //se o processador é o mesmo do anterior
		{
			timestamp[TASK[i]] = totaltime[PROCESSOR[i]];
			totaltime[PROCESSOR[i]] += grafo.nodes[TASK[i]].cost;
		}
		else
		{
			gettasktime(ind, i, totaltime, timestamp);
		}
		//printf("task: %2d timestamp: %d\n",TASK[i],timestamp[TASK[i]]);
	}
	
	ind->fitness = max(totaltime[0],totaltime[1]);
	return ind->fitness;
}