#include "../genalg/genalg.h"


//gera dois indivíduos filhos dados dois pais
Individual* cyclecrossover(Individual *p1,Individual *p2)
{
	int i,j,k,inicial,aux[2];
	
	c[0] = allocateindividual();
	c[1] = allocateindividual();

	//copia os individuos
	for(i=0; i<grafo.n; i++)
	{
		c[0]->sequence[i] = p1->sequence[i];
		c[0]->processors[i] = p1->processors[i];
		c[1]->sequence[i] = p2->sequence[i];
		c[1]->processors[i] = p2->processors[i];
	}

	k = rand()%grafo.n;

	//printf("k = %d\n",k);

	aux[0] = c[0]->sequence[k];
	aux[1] = c[0]->processors[k];
	c[0]->sequence[k] = c[1]->sequence[k];
	c[0]->processors[k] = c[1]->processors[k];
	c[1]->sequence[k] = aux[0];
	c[1]->processors[k] = aux[1];

	inicial = c[1]->sequence[k];
	i = c[0]->sequence[k];
	while(i!=inicial)
	{
		for(j=0;j<grafo.n;j++)
		{
			if(j!=k && c[0]->sequence[j]==i)
				break;
		}

		aux[0] = c[0]->sequence[j];
		aux[1] = c[0]->processors[j];
		c[0]->sequence[j] = c[1]->sequence[j];
		c[0]->processors[j] = c[1]->processors[j];
		c[1]->sequence[j] = aux[0];
		c[1]->processors[j] = aux[1];

		k = j;
		i = c[0]->sequence[j];
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
		c[0]->sequence[j] = p1->sequence[j];
		c[0]->processors[j] = p2->processors[j];
		c[1]->sequence[j] = p2->sequence[j];
		c[1]->processors[j] = p1->processors[j];
	}

	return *c;
}