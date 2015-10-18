#ifndef GENALG_H
#define GENALG_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "datastructures.h"
#include "graph.h"


Graph grafo;


void getgraph(char* filename);

list newlist(int cap);
void add(list l,int a);
int at(list l,int index);
void erase(list l,int index);

#endif