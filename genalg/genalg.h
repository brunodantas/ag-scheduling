#ifndef GENALG_H
#define GENALG_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>
#include "../domain/datastructures.h"
#include "../domain/graph.h"


int POPSIZE;
int NEXTGENSIZE;
int MUTATIONRATE; //percent
int MAXGENERATIONS;
int PROCESSORQTY;
int NPOPS;
int MYRANK;
int MIGRATIONFREQ;
int MIGRATIONRATE;
Graph grafo;
Population population;
Individual *bestindividual;
Individual **recyclelist;
int recyclelistsize;
int init;
int tournamentsize;
Individual* c[2];
Individual* (*selection)(void);
Individual* (*crossover)(Individual*,Individual*);
void (*reinsertion)(Population);
void (*migration)(int);
char problema[50];


void getgraph(char* filename);

Individual* genalg();
void getinput();

Individual* newindividual();
int evaluate(Individual *ind);
void gettasktime(Individual *ind, int taskindex, int* totaltime, int* timestamp);
void mutation(Individual *ind);
void mutation2(Individual *ind);
void makevalid(Individual *ind);
Individual* allocateindividual();
int compareind(const void * a,const void * b);

Population* initpopulation();
Population nextgeneration();
void best();

Individual* tournament();
Individual* random_selection();

Individual* cyclecrossover(Individual *p1,Individual *p2);
Individual* newcrossover(Individual *p1,Individual *p2);

void bestreinsertion(Population nextgen);
void elite1reinsertion(Population nextgen);
void elitism(Population nextgen);

list newlist(int cap);
void add(list l,int a);
int at(list l,int index);
void erase(list l,int index);

void init_mpop();
void ring_migration(int generation);
Individual* best_found();

#endif