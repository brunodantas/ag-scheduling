//arquivo para teste de componentes etc


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


//gera um indivíduo válido
void testindividual1()
{
	int i;
	getgraph("gauss18.txt");
	Individual* ind = newindividual();

	printf("tasks: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[0][i]);
	printf("\nprocs:");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[1][i]);
	printf("\n");
}


//leitura de grafo de um arquivo e inicialização da estrutura
void testgraph1()
{
	int i;
	getgraph("gauss18.txt");
	Edge e;
	Node* no;

	for(i=0;i<grafo.n;i++)
	{
		no = &grafo.nodes[i];
		printf("Nó %d\n\tSucessores: ",no->id);
		for(e=no->successors; e!=NULL; e=e->next)
		{
			printf("%d, ",e->node->id);
		}
		printf("\n\tAntecessores: ");
		for(e=no->predecessors; e!=NULL; e=e->next)
		{
			printf("%d, ",e->node->id);
		}
		printf("\n\tNumero de antecessores: %d\n",no->predqty);
	}
}


int main()
{
	srand(time(NULL));
	testindividual1();
	return 0;
}