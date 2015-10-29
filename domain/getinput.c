#include "../genalg/genalg.h"


//leitura de grafo de um arquivo e inicialização da estrutura
void getgraph(char* filename)
{
	FILE *file;
	int i,z;
	int n,a,b,c;

	file = fopen(filename,"r");

	//lê o número de nós
	z = fscanf(file,"%d\n",&n);
	initgraph(&grafo,n);

	//lê os custos dos nós
	for(i=0;i<grafo.n-1;i++)
	{
		z = fscanf(file,"%d,",&grafo.nodes[i].cost);
	}
	z = fscanf(file,"%d\n",&grafo.nodes[i].cost);

	//lê as arestas
	while(fscanf(file,"%d,%d %d\n",&a,&b,&c)!=EOF)
	{
		addedge(&grafo,a,b,c);
	}
}


//leitura do arquivo input.txt
void getinput()
{
	int i;
	FILE *f;
	int s,c,r,e,nge,ex;
	char problema[50];

	Individual* (*sel[4])(void) = {&roullete,&tournament3,&tournament2};
	Individual* (*cross[2])(Individual*,Individual*) = {&cyclecrossover};
	void (*reins[2])(Population) = {&bestreinsertion};

	f = fopen("input.txt","r");
	i = fscanf(f,"Problema: %s\n\n",problema);
	getgraph(problema);
	i = fscanf(f,"S%d, C%d, R%d\n\n",&s,&c,&r);
	i = fscanf(f,"Population: %d\nGenerations: %d\nCrossover: %d%%\nMutation: %d%%\n",&POPSIZE,&MAXGENERATIONS,&NEXTGENSIZE,&MUTATIONRATE);
	NEXTGENSIZE *= POPSIZE;
	NEXTGENSIZE /= 100;

	selection = sel[s-1];
	crossover = cross[c-1];
	reinsertion = reins[r-1];
}