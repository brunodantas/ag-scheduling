

typedef struct
{
		int *traits[2]; //sequência de duplas tarefa/processador
		int fitness; //aptidão do indivíduo (menor é melhor)
}Individual;

typedef Individual** Population;


struct l
{
	int size;
	int *info;
};
typedef struct l* list;


struct pl
{
	Population info;
	int size;
};
typedef struct pl* poplist;