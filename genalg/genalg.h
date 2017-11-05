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
int CONF;
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
Population (*nextgeneration)(void);
char problema[50];


void getgraph(char* filename);

Individual* genalg();
void getinput();

Individual* newindividual();
int evaluate(Individual *ind);
void gettasktime(Individual *ind, int taskindex, int* totaltime, int* timestamp);
void mutation(Individual *ind);
void mutation_swap(Individual *ind);
void mutation_proc(Individual *ind);
void makevalid(Individual *ind);
Individual* allocateindividual();
int compareind(const void * a,const void * b);

Population* initpopulation();
void best();

Individual* roullete();
Individual* tournament();
Individual* random_selection();

Individual* cycle_crossover_seq(Individual *p1,Individual *p2);
Individual* cycle_crossover_carry(Individual *p1,Individual *p2);
Individual* exchange_crossover(Individual *p1,Individual *p2);
Individual* one_point_seq_crossover(Individual *p1,Individual *p2);
Individual* two_point_seq_crossover(Individual *p1,Individual *p2);
Individual* uniform_seq_crossover(Individual *p1,Individual *p2);
Individual* uniform_crossover_carry(Individual *p1,Individual *p2);
Individual* one_point_proc_crossover(Individual *p1,Individual *p2);
Individual* two_point_proc_crossover(Individual *p1,Individual *p2);
Individual* uniform_proc_crossover(Individual *p1,Individual *p2);
Individual* one_point_both_crossover(Individual *p1,Individual *p2);
Individual* two_point_both_crossover(Individual *p1,Individual *p2);
Individual* uniform_both_crossover(Individual *p1,Individual *p2);
Individual* ox_seq(Individual *p1,Individual *p2);
Individual* ox_carry(Individual *p1,Individual *p2);
Individual* pmx_carry(Individual *p1,Individual *p2);
Individual* pmx_seq(Individual *p1,Individual *p2);
Individual* cycle_two_point(Individual *p1,Individual *p2);
Individual* pmx_two_point(Individual *p1,Individual *p2);
Individual* ox_two_point(Individual *p1,Individual *p2);

void bestreinsertion(Population nextgen);
void elite1reinsertion(Population nextgen);
void elitism(Population nextgen);

void init_config(int conf);
Population run_config();
Population config0();
Population config1();
Population config2();
Population config3();
Population config4();
Population config5();
Population config6();
Population config7();
Population config8();
Population config9();
Population config10();
Population config11();

list newlist(int cap);
void add(list l,int a);
int at(list l,int index);
void erase(list l,int index);

void init_mpop();
void ring_migration(int generation);
Individual* best_found();

#endif