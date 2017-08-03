#include "../genalg/genalg.h"


//segundo padrao de input
void getgraph(char* filename)
{
	FILE *file;
	int i,z;
	int n,a,b,c;

	file = fopen(filename,"r");

	sscanf(filename, "%*[^0123456789]%d.txt",&n);
	initgraph(&grafo,n);

	//lê os custos dos nós
	for(i=0;i<grafo.n;i++)
	{
		z = fscanf(file,"%d\n",&grafo.nodes[i].cost);
	}
	z = fscanf(file,"%d\n",&a);

	//lê as arestas
	while(fscanf(file,"%d %d %d\n",&a,&b,&c)!=EOF)
	{
		addedge(&grafo,a,b,c);
	}
}


//leitura de grafo de um arquivo e inicialização da estrutura
void getgraph2(char* filename)
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
	while(fscanf(file,"%d,%d,%d\n",&a,&b,&c)!=EOF)
	{
		addedge(&grafo,a,b,c);
	}
}




//leitura do arquivo input.txt
void getinput()
{
	int i;
	FILE *f;
	char problema[50];

	f = fopen("input.txt","r");
	i = fscanf(f,"Problem: %s\n\n",problema);
	getgraph(problema);
	i = fscanf(f,"Processors: %d\nPopulation: %d\nGenerations: %d\nCrossover: %d%%\nMutation: %d%%\nTournament: %d\n\nMigrationFreq: %d\nMigrationRate: %d%%",
		&PROCESSORQTY,&POPSIZE,&MAXGENERATIONS,&NEXTGENSIZE,&MUTATIONRATE,&tournamentsize,&MIGRATIONFREQ,&MIGRATIONRATE);
	
	// selection = &tournament;
	crossover = &cyclecrossover;
	reinsertion = &bestreinsertion;
}