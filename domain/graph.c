#include "graph.h"


//inicializa nós do grafo
void initgraph(Graph* grafo, int n)
{
	int i;
	Node* node;

	grafo->n = n;
	grafo->nodes = (Node*) malloc(n*sizeof(Node));

	for(i=0;i<n;i++)
	{
		node = &grafo->nodes[i];
		node->id = i;
		node->predqty = 0;
		node->successors = NULL;
		node->predecessors = NULL;
	}
}


//adiciona aresta no grafo
void addedge(Graph* grafo, int a, int b, int cost)
{
	Edge e = (Edge) malloc(sizeof(struct e));
	Edge e2 = (Edge) malloc(sizeof(struct e));
	Node* A = &grafo->nodes[a];
	Node* B = &grafo->nodes[b];

	e->node = B;
	e->cost = cost;
	e->next = A->successors;
	A->successors = e;

	e2->node = A;
	e2->cost = cost;
	e2->next = B->predecessors;
	B->predecessors = e2;
	B->predqty++;
}