#ifndef GENALG_H
#define GENALG_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../domain/datastructures.h"
#include "../domain/graph.h"


int POPSIZE;
int NEXTGENSIZE;
int MUTATIONRATE; //percent
int MAXGENERATIONS;
Graph grafo;
Population population;
Individual *bestindividual;
Individual **recyclelist;
int recyclelistsize;
int init;
Individual* c[2];
Individual* (*selection)(void);
Individual* (*crossover)(Individual*,Individual*);
void (*reinsertion)(Population);


void getgraph(char* filename);

Individual* genalg();

Individual* newindividual();
int evaluate(Individual *ind);
void mutation(Individual *ind);
void makevalid(Individual *ind);
void initrecyclelist();
Individual* getrecycledindividual();
void recycleindividual(Individual* ind);
void recyclepopulation();

Population* initpopulation();
Population nextgeneration();
void best();

Individual* roullete();
Individual* tournament3();
Individual* tournament2();

Individual* cyclecrossover(Individual *p1,Individual *p2);

void bestreinsertion(Population nextgen);

list newlist(int cap);
void add(list l,int a);
int at(list l,int index);
void erase(list l,int index);


#endif