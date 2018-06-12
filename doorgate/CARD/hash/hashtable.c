#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "hash.h"
#include "hashtable.h"



int InitHashTable(hashdata*** HashTable,int size)
{	
	*HashTable = (hashdata**) malloc(size * sizeof(hashdata*));
	if (NULL == *HashTable)
	{
		return 0;
	}
	memset(*HashTable,0,size * sizeof(hashdata*));
	return 1;
}

void DestroyHashTable(hashdata*** HashTable,int size)
{
	int i;
	hashdata* p;
	for (i = 0; i < size; i++)	
	{
		p = (*HashTable)[i];
		HashDataFree(&p);
	}

	free(*HashTable);
	*HashTable = NULL;
}

hashdata* CreateDataNode(const char* data)
{
	hashdata* p = (hashdata*)malloc(sizeof(hashdata));
	assert(p != NULL);
	p->next = NULL;
	p->data = (char*)malloc(strlen(data));
	assert(p->data);
	strcpy(p->data,data);

	return p;
}

void HashDataFree(hashdata** L)
{
	if ((*L) != NULL)
	{
		HashDataFree(&((*L)->next));
		if ((*L)->data != NULL) 
		{
			free((*L)->data);
			(*L)->data = NULL;
		}
		free(*L);
		*L = NULL;
	}
}

int AddData(hashdata* HashTable[],int size,const char* data)
{
	unsigned int slot;		
	unsigned int tmp;	

	tmp = HashPJW1(data);								

	printf("slot = %d\n",tmp); 	
	
	slot = tmp%size;					

	hashdata* current = HashTable[slot];
	hashdata* before = current;
	while (current)
	{	
		before = current;
		if (strcmp(current->data,data) == 0)
		{
			printf("%s is already exist!\n",data);
			return 0;
		}
		else
		{	
			current = current->next; 
		}
	}

	if (before == NULL)
		HashTable[slot]	= CreateDataNode(data);
	else
		before->next = CreateDataNode(data);

	return 1;
}

int DeleteData(hashdata* HashTable[],int size,const char* data)
{
	int slot = HashPJW1(data) % size;

	if (HashTable[slot] == NULL)
	{
		return 0;
	}	

	if (strcmp((HashTable[slot])->data,data) == 0)
	{
		hashdata* after = (HashTable[slot])->next;
		free((HashTable[slot])->data);
		free(HashTable[slot]);
		HashTable[slot] = after;
		return 1;
	}
	
	hashdata* current = HashTable[slot];
	hashdata* before = HashTable[slot];

	current = current->next;
	while (current)
	{
		if (strcmp(current->data,data) == 0)
		{
			break;
		}
		else
		{
			before = before->next;
			current = current->next; 
		}
	}

	if (current == NULL)
	{
		return 0;
	}
	else
	{
		before->next = current->next;
		free(current->data);
		current->data = NULL;
		free(current);
		current = NULL;
	}
	
	return 1;
}

int FindData(hashdata* HashTable[], int size,const char* data)
{		
	int slot = HashPJW1(data) % size;
	hashdata* p = HashTable[slot];

	while (p)
	{
		if (strcmp(p->data,data) == 0)
		{
			return 1;
		}

		p = p->next;
	}
	
	return 0;
}

void PrintHashTable(hashdata* HashTable[],int size)
{
	int i;
	hashdata* p;
	for (i = 0; i < size; i++)
	{
		printf ("[%d]->",i);
		p = HashTable[i];
		while (p != NULL)
		{
			printf("%s",p->data);
			if ((p = p->next))
				printf("->");
		}
		printf("\n");
	}
}










