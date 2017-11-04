#include "../genalg/genalg.h"


//gera dois indivíduos filhos dados dois pais
Individual* cycle_crossover(Individual *p1,Individual *p2, int carry)
{
	int i,j,k,inicial,aux,task;
	
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

	aux = c[0]->sequence[k];
	c[0]->sequence[k] = c[1]->sequence[k];	
	c[1]->sequence[k] = aux;
	if(carry)
	{
		task = c[0]->sequence[k];
		aux = c[0]->processors[task];
		c[0]->processors[task] = c[1]->processors[task];
		c[1]->processors[task] = aux;
	}

	inicial = c[1]->sequence[k];
	i = c[0]->sequence[k];
	while(i!=inicial)
	{
		for(j=0;j<grafo.n;j++)
		{
			if(j!=k && c[0]->sequence[j]==i)
				break;
		}

		aux = c[0]->sequence[j];
		c[0]->sequence[j] = c[1]->sequence[j];
		c[1]->sequence[j] = aux;
		if(carry)
		{
			task = c[0]->sequence[k];
			aux = c[0]->processors[task];
			c[0]->processors[task] = c[1]->processors[task];
			c[1]->processors[task] = aux;
		}

		k = j;
		i = c[0]->sequence[j];
	}
	makevalid(c[0]);
	makevalid(c[1]);
	return *c;
}


Individual* cycle_crossover_seq(Individual *p1,Individual *p2)
{
	return cycle_crossover(p1,p2,0);
}


Individual* cycle_crossover_carry(Individual *p1,Individual *p2)
{
	return cycle_crossover(p1,p2,1);
}


Individual* exchange_crossover(Individual *p1,Individual *p2)
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


Individual* ox(Individual *p1,Individual *p2, int start, int end, int carry)
{
	Individual* child = allocateindividual();
	int i,j,task,p,count,a,b;
	int* used = calloc(grafo.n, sizeof(int));
	int* predecessor = calloc(grafo.n, sizeof(int));
	int* successor = calloc(grafo.n, sizeof(int));
	int predecessorsleft = 0;
	Edge e;

	if(!carry)
	{
		a = 0;
		b = grafo.n;
	}
	else
	{
		a = start;
		b = end;		
	}

	for(i=a;i<b;i++)
	{
		task = p1->sequence[i];
		child->processors[task] = p1->processors[task];
	}

	for(i=start;i<end;i++)
	{
		used[p1->sequence[i]] = 1;
		successor[p1->sequence[i]] = 1;
	}

	//contar predecessores do bloco
	for(i=start;i<end;i++)
	{
		task = p1->sequence[i];
		for(e = grafo.nodes[task].predecessors; e!=NULL; e = e->next)
		{
			p = e->node->id;
			if(!used[p] && predecessor[p] == 0)
			{
				predecessor[p] = 1;
				predecessorsleft++;
			}
		}
	}

	//contar sucessores do bloco
	for(i=end;i<grafo.n;i++)
	{
		task = p1->sequence[i];
		for(e = grafo.nodes[task].predecessors; e!=NULL; e = e->next)
		{
			p = e->node->id;
			if(successor[p]) //se um predecessor de task é um sucessor do bloco
			{				 //então task também é sucessor
				successor[task] = 1;
				break;
			}
		}
	}

	//porção anterior ao bloco
	count = 0;
	for(i=0;count < start || predecessorsleft > 0;i++)
	{
		task = p2->sequence[i];
		if(!successor[task]) //se task não pertence ao bloco
		{					 //e não é sucessor do bloco

			child->sequence[count] = task;
			used[task] = 1;
			if(carry)
				child->processors[task] = p2->processors[task];
			if(predecessor[task])
				predecessorsleft--;
			count++;
		}
	}

	//bloco
	for(i=start;i < end;i++)
	{
		child->sequence[count] = p1->sequence[i];
		count++;
	}

	//resto
	for(i=0;count < grafo.n;i++)
	{
		task = p2->sequence[i];
		if(!used[task])
		{
			child->sequence[count] = task;
			if(carry)
				child->processors[task] = p2->processors[task];
			count++;
		}
	}

	free(used);
	free(predecessor);
	free(successor);
	return child;
}


Individual* ox_seq(Individual *p1,Individual *p2)
{
	int start = rand()%(grafo.n - 4);
	int end   = rand()%(grafo.n - start) + start + 1;
	c[0] = ox(p1,p2,start,end,0);
	c[1] = ox(p2,p1,start,end,0);
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",p1->sequence[j],p1->processors[p1->sequence[j]]);
	// }
	// printf("\n");
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",c[0]->sequence[j],c[0]->processors[c[0]->sequence[j]]);
	// }
	// printf("\n");
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",p2->sequence[j],p2->processors[p2->sequence[j]]);
	// }
	// printf("\n");
	// for(int j=0;j<grafo.n;j++)
	// {
	// 	printf("%d/%d, ",c[1]->sequence[j],c[1]->processors[c[1]->sequence[j]]);
	// }
	// printf("\n%d,%d\n",start,end);
	return *c;
}


Individual* ox_carry(Individual *p1,Individual *p2)
{
	int start = rand()%(grafo.n - 4);
	int end   = rand()%(grafo.n - start) + start + 1;
	c[0] = ox(p1,p2,start,end,1);
	c[1] = ox(p2,p1,start,end,1);
	return *c;
}