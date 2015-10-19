#ifndef GENALG_H
#define GENALG_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "datastructures.h"
#include "graph.h"


int POPSIZE;
int NEXTGENSIZE;
int MUTATIONRATE; //percent
int MAXGENERATIONS;
Graph grafo;
Population population;
Individual *bestindividual;


void getgraph(char* filename);

Individual* newindividual();
int evaluate(Individual *ind);

Population* initpopulation();
void populationinsert(Population population,Individual *ind,int size);

list newlist(int cap);
void add(list l,int a);
int at(list l,int index);
void erase(list l,int index);


#endif