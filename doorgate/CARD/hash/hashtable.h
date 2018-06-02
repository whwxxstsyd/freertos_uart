#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <string.h>

typedef struct _hashdata
{
	char* data;
	struct _hashdata *next;
} hashdata; 

int InitHashTable(hashdata*** HashTable,int size);
void DestroyHashTable(hashdata*** HashTable,int size);

int AddData(hashdata* HashTable[],int size,const char* data);
int DeleteData(hashdata* HashTable[],int size,const char* data);
int FindData(hashdata* HashTable[], int size,const char* data);

void PrintHashTable(hashdata* HashTable[],int size);

hashdata* CreateDataNode(const char* data);
void HashDataFree(hashdata** L);


#endif /*HASHTABLE_H*/
