

typedef struct
{
	int *sequence; //task sequence
	int *processors; //task-processor mapping 
	int fitness; //less is better
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