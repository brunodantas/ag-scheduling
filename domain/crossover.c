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


Individual* one_point_seq_crossover(Individual *p1,Individual *p2)
{
	c[0] = allocateindividual();
	c[1] = allocateindividual();
	int point = rand()%(grafo.n - 3) + 1;
	int* used[2];
	used[0] = calloc(grafo.n, sizeof(int));
	used[1] = calloc(grafo.n, sizeof(int));
	int i,j,k,task;

	for(i=0;i<point;i++)
	{
		task = p1->sequence[i];
		c[0]->sequence[i] = task;
		used[0][task] = 1;

		task = p2->sequence[i];
		c[1]->sequence[i] = task;
		used[1][task] = 1;
	}

	j=i;
	k=i;
	for(i=0;i<grafo.n;i++)
	{
		task = p2->sequence[i];
		if(j < grafo.n && !used[0][task])
		{
			c[0]->sequence[j] = task;
			j++;
		}

		task = p1->sequence[i];
		if(k < grafo.n && !used[1][task])
		{
			c[1]->sequence[k] = task;
			k++;
		}
	}

	for(i=0;i<grafo.n;i++)
	{
		c[0]->processors[i] = p1->processors[i];
		c[1]->processors[i] = p2->processors[i];
	}
	free(used[0]);
	free(used[1]);
	return *c;
}


Individual* two_point_seq_crossover(Individual *p1,Individual *p2)
{
	c[0] = allocateindividual();
	c[1] = allocateindividual();
	int point1 = rand()%(grafo.n - 4) + 1;
	int point2 = rand()%(grafo.n - point1-1) + point1 + 1;
	int* used[2];
	used[0] = calloc(grafo.n, sizeof(int));
	used[1] = calloc(grafo.n, sizeof(int));
	int i,j,k,l,task;

	for(i=0;i<point1;i++)
	{
		task = p1->sequence[i];
		c[0]->sequence[i] = task;
		used[0][task] = 1;

		task = p2->sequence[i];
		c[1]->sequence[i] = task;
		used[1][task] = 1;
	}

	j=i;
	k=i;
	l=i;
	for(i=0;i<point2;i++)
	{
		task = p2->sequence[i];
		if(j < grafo.n && !used[0][task])
		{
			c[0]->sequence[j] = task;
			used[0][task] = 1;
			j++;
		}

		task = p1->sequence[i];
		if(k < grafo.n && !used[1][task])
		{
			c[1]->sequence[k] = task;
			used[1][task] = 1;
			k++;
		}
	}

	for(i=l;i<grafo.n;i++)
	{
		task = p1->sequence[i];
		if(j < grafo.n && !used[0][task])
		{
			c[0]->sequence[j] = task;
			j++;
		}

		task = p2->sequence[i];
		if(k < grafo.n && !used[1][task])
		{
			c[1]->sequence[k] = task;
			k++;
		}
	}
	for(i=0;i<grafo.n;i++)
	{
		c[0]->processors[i] = p1->processors[i];
		c[1]->processors[i] = p2->processors[i];
	}
	free(used[0]);
	free(used[1]);
	return *c;
}


Individual* uniform_seq_crossover(Individual *p1,Individual *p2)
{
	c[0] = allocateindividual();
	c[1] = allocateindividual();
	int i,r,task;
	int j[2],k[2];
	j[0]=0; j[1]=0; k[0]=0; k[1]=0;

	Individual* p[2];
	p[0] = p1;
	p[1] = p2;

	int* used[2];
	used[0] = calloc(grafo.n, sizeof(int));
	used[1] = calloc(grafo.n, sizeof(int));

	for(i=0;i<grafo.n;i++)
	{
		c[0]->processors[i]   = p[0]->processors[i];
		c[1]->processors[i]   = p[1]->processors[i];

		r = rand()%2;

		task = p[r]->sequence[j[r]];
		while(used[0][task])
		{
			task = p[r]->sequence[j[r]];
			j[r]++;
		}
		c[0]->sequence[i] = task;
		used[0][task] = 1;
		
		task = p[!r]->sequence[k[r]];
		while(used[1][task])
		{
			task = p[!r]->sequence[k[r]];
			k[r]++;
		}
		c[1]->sequence[i] = task;
		used[1][task] = 1;
	}


	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",p1->sequence[j],p1->processors[j]);
	// }
	// printf("\n");
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",c[0]->sequence[j],c[0]->processors[j]);
	// }
	// printf("\n");
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",p2->sequence[j],p2->processors[j]);
	// }
	// printf("\n");
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",c[1]->sequence[j],c[1]->processors[j]);
	// }
	// printf("\n\n");//,point1,point2);
	
	free(used[0]);
	free(used[1]);
	return *c;
}


Individual* uniform_both_crossover(Individual *p1,Individual *p2)
{
	c[0] = allocateindividual();
	c[1] = allocateindividual();
	int i,r,task;
	int j[2],k[2];
	j[0]=0; j[1]=0; k[0]=0; k[1]=0;

	Individual* p[2];
	p[0] = p1;
	p[1] = p2;

	int* used[2];
	used[0] = calloc(grafo.n, sizeof(int));
	used[1] = calloc(grafo.n, sizeof(int));

	for(i=0;i<grafo.n;i++)
	{
		r = rand()%2;

		task = p[r]->sequence[j[r]];
		while(used[0][task])
		{
			task = p[r]->sequence[j[r]];
			j[r]++;
		}
		c[0]->sequence[i] = task;
		used[0][task] = 1;
		
		task = p[!r]->sequence[k[r]];
		while(used[1][task])
		{
			task = p[!r]->sequence[k[r]];
			k[r]++;
		}
		c[1]->sequence[i] = task;
		used[1][task] = 1;
	}

	for(i=0;i<grafo.n;i++)
	{
		r = rand()%2;
		c[0]->processors[i] = p[r]->processors[i];
		c[1]->processors[i] = p[!r]->processors[i];
	}	


	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",p1->sequence[j],p1->processors[j]);
	// }
	// printf("\n");
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",c[0]->sequence[j],c[0]->processors[j]);
	// }
	// printf("\n");
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",p2->sequence[j],p2->processors[j]);
	// }
	// printf("\n");
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",c[1]->sequence[j],c[1]->processors[j]);
	// }
	// printf("\n\n");//,point1,point2);
	
	free(used[0]);
	free(used[1]);
	return *c;
}


Individual* one_point_proc_crossover(Individual *p1,Individual *p2)
{
	c[0] = allocateindividual();
	c[1] = allocateindividual();
	int point = rand()%(grafo.n - 2) + 1;
	int i;

	for(i=0;i<point;i++)
	{
		c[0]->sequence[i]   = p1->sequence[i];
		c[0]->processors[i] = p1->processors[i];

		c[1]->sequence[i]   = p2->sequence[i];
		c[1]->processors[i] = p2->processors[i];
	}

	for(;i<grafo.n;i++)
	{
		c[0]->sequence[i]   = p1->sequence[i];
		c[0]->processors[i] = p2->processors[i];

		c[1]->sequence[i]   = p2->sequence[i];
		c[1]->processors[i] = p1->processors[i];
	}
	return *c;
}


Individual* two_point_proc_crossover(Individual *p1,Individual *p2)
{
	c[0] = allocateindividual();
	c[1] = allocateindividual();
	int point1 = rand()%(grafo.n - 4) + 1;
	int point2 = rand()%(grafo.n - point1-1) + point1 + 1;
	int i;

	for(i=0;i<point1;i++)
	{
		c[0]->sequence[i]   = p1->sequence[i];
		c[0]->processors[i] = p1->processors[i];

		c[1]->sequence[i]   = p2->sequence[i];
		c[1]->processors[i] = p2->processors[i];
	}

	for(;i<point2;i++)
	{
		c[0]->sequence[i]   = p1->sequence[i];
		c[0]->processors[i] = p2->processors[i];

		c[1]->sequence[i]   = p2->sequence[i];
		c[1]->processors[i] = p1->processors[i];
	}

	for(;i<grafo.n;i++)
	{
		c[0]->sequence[i]   = p1->sequence[i];
		c[0]->processors[i] = p1->processors[i];

		c[1]->sequence[i]   = p2->sequence[i];
		c[1]->processors[i] = p2->processors[i];
	}
	return *c;
}


Individual* uniform_proc_crossover(Individual *p1,Individual *p2)
{
	c[0] = allocateindividual();
	c[1] = allocateindividual();
	int i,r;
	Individual* p[2];
	p[0] = p1;
	p[1] = p2;

	for(i=0;i<grafo.n;i++)
	{
		c[0]->sequence[i]   = p[0]->sequence[i];
		c[1]->sequence[i]   = p[1]->sequence[i];

		r = rand()%2;
		c[0]->processors[i] = p[r]->processors[i];
		c[1]->processors[i] = p[!r]->processors[i];
	}	
	return *c;
}


Individual* one_point_both_crossover(Individual *p1,Individual *p2)
{
	c[0] = allocateindividual();
	c[1] = allocateindividual();
	int point  = rand()%(grafo.n - 3) + 1;
	int point2 = rand()%(grafo.n - 2) + 1;
	int* used[2];
	used[0] = calloc(grafo.n, sizeof(int));
	used[1] = calloc(grafo.n, sizeof(int));
	int i,j,k,task;

	for(i=0;i<point;i++)
	{
		task = p1->sequence[i];
		c[0]->sequence[i] = task;
		used[0][task] = 1;

		task = p2->sequence[i];
		c[1]->sequence[i] = task;
		used[1][task] = 1;
	}

	j=i;
	k=i;
	for(i=0;i<grafo.n;i++)
	{
		task = p2->sequence[i];
		if(j < grafo.n && !used[0][task])
		{
			c[0]->sequence[j] = task;
			j++;
		}

		task = p1->sequence[i];
		if(k < grafo.n && !used[1][task])
		{
			c[1]->sequence[k] = task;
			k++;
		}
	}

	for(i=0;i<point2;i++)
	{
		c[0]->processors[i] = p1->processors[i];
		c[1]->processors[i] = p2->processors[i];
	}

	for(;i<grafo.n;i++)
	{
		c[0]->processors[i] = p2->processors[i];
		c[1]->processors[i] = p1->processors[i];
	}

	free(used[0]);
	free(used[1]);
	return *c;
}