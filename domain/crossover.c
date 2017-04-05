#include "../genalg/genalg.h"


//gera dois indivíduos filhos dados dois pais
Individual* cyclecrossover(Individual *p1,Individual *p2)
{
	int i,j,k,inicial,aux[2];
	
	c[0] = allocateindividual();
	c[1] = allocateindividual();

	//copia os individuos
	for(i=0;i<2;i++)
	{
		for(j=0; j<grafo.n; j++)
		{
			c[0]->traits[i][j] = p1->traits[i][j];
			c[1]->traits[i][j] = p2->traits[i][j];
		}
	}

	k = rand()%grafo.n;

	//printf("k = %d\n",k);

	aux[0] = c[0]->traits[0][k];
	aux[1] = c[0]->traits[1][k];
	c[0]->traits[0][k] = c[1]->traits[0][k];
	c[0]->traits[1][k] = c[1]->traits[1][k];
	c[1]->traits[0][k] = aux[0];
	c[1]->traits[1][k] = aux[1];

	inicial = c[1]->traits[0][k];
	i = c[0]->traits[0][k];
	while(i!=inicial)
	{
		for(j=0;j<grafo.n;j++)
		{
			if(j!=k && c[0]->traits[0][j]==i)
				break;
		}

		aux[0] = c[0]->traits[0][j];
		aux[1] = c[0]->traits[1][j];
		c[0]->traits[0][j] = c[1]->traits[0][j];
		c[0]->traits[1][j] = c[1]->traits[1][j];
		c[1]->traits[0][j] = aux[0];
		c[1]->traits[1][j] = aux[1];

		k = j;
		i = c[0]->traits[0][j];
	}
	makevalid(c[0]);
	makevalid(c[1]);
	return *c;
}


Individual* newcrossover(Individual *p1,Individual *p2)
{
	int j;
	c[0] = allocateindividual();
	c[1] = allocateindividual();

	for(j=0; j<grafo.n; j++)
	{
		c[0]->traits[0][j] = p1->traits[0][j];
		c[0]->traits[1][j] = p2->traits[1][j];
		c[1]->traits[0][j] = p2->traits[0][j];
		c[1]->traits[1][j] = p1->traits[1][j];
	}

	return *c;
}