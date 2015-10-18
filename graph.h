#ifndef GRAPH_H
#define GRAPH_H


#include <stdlib.h>


typedef struct e* Edge;
typedef struct n Node;


typedef struct
{
	int n; //quantidade de nós
	Node* nodes;
}Graph;


struct n
{
	int id;
	int cost;
	int predqty; //quantidade de predecessores
	Edge successors;
	Edge predecessors;
};


struct e
{
	Node* node;
	int cost;
	Edge next;
};


void initgraph(Graph* grafo, int n);
void addedge(Graph* grafo, int a, int b, int cost);


#endif