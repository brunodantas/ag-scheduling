//arquivo para teste de componentes etc


#include "genalg.h"


void testinitpopulation()
{
	int i,j;
	getgraph("gauss18.txt");
	POPSIZE = 50;
	NEXTGENSIZE = 30;
	initpopulation();
	Individual* ind;

	printf("Initial Population:\n");
	for(i=0;i<POPSIZE;i++)
	{
		ind = population[i];
		printf("Individual %d\nTraits: ",i);
		for(j=0;j<grafo.n;j++)
		{
			printf("%d/%d, ",ind->traits[0][j],ind->traits[1][j]);
		}
		printf("\nFitness: %d\n\n",ind->fitness);
	}
}


//testa a função fitness com indivíduo aleatório para grafoteste.txt
void testfitness2()
{
	int i;
	getgraph("grafoteste.txt");
	Individual* ind = newindividual();

	printf("tasks: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[0][i]);
	printf("\nprocs: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[1][i]);
	printf("\n");
	printf("\nFitness: %d\n",evaluate(ind));
}


//testa a função fitness com solução ótima para o gauss18
void testfitness1()
{
	int i;
	getgraph("gauss18.txt");
	Individual* ind = newindividual();

	int otima[2][18] = {{0,2,6,3,5,8,1,11,4,10,7,9,13,15,16,12,14,17},{1,1,1,1,0,1,0,1,1,0,0,1,1,1,1,0,1,1}};

	for(i=0;i<grafo.n;i++)
	{
		ind->traits[0][i] = otima[0][i];
		ind->traits[1][i] = otima[1][i];
	}

	printf("tasks: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[0][i]);
	printf("\nprocs: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[1][i]);
	printf("\n");
	printf("\nFitness: %d\n",evaluate(ind));
}


//gera um indivíduo válido e exibe sua aptidão
void testindividual2()
{
	int i;
	getgraph("gauss18.txt");
	Individual* ind = newindividual();

	printf("tasks: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[0][i]);
	printf("\nprocs: ");
	for(i=0;i<grafo.n;i++)
		printf("%d,",ind->traits[1][i]);
	printf("\nFitness: %d\n",evaluate(ind));
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
	printf("\nprocs: ");
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
	testinitpopulation();
	return 0;
}