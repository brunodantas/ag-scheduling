#include "genalg.h"


list newlist(int cap)
{
	list l = malloc(sizeof(struct l));
	l->size = 0;
	l->info = malloc(cap*sizeof(int));
	return l;
}

void add(list l,int a)
{
	l->info[l->size] = a;
	l->size++;
}

int at(list l,int index)
{
	return l->info[index];
}

void erase(list l,int index)
{
	int i;

	for(i=index; i < l->size-1; i++)
	{
		l->info[i] = l->info[i+1];
	}
	l->size--;
}