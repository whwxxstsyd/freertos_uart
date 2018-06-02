#include <stdlib.h>
#include <stdio.h>
#include "hashtable.h"

#define N 256				

int Hah_Test(void)	
{
	//声明一个指针数组
	hashdata** ht;

	char s[20];		
		
	InitHashTable(&ht,N);
	
	const char* keycode[15] = {		
		"01234567","11234567","21234567","31234567","41234567",
		"51234567","61234567","71234567","81234567","91234567",
		"15234567","10234567","14234567","12234567","13234567"					
	};
			
	int i;
	for (i = 0; i < 15; i++)
	{
		AddData(ht,N,keycode[i]);
	}
	PrintHashTable(ht,N);

	printf("\n");

	printf("please input the number you want to find\n");	
	
	gets(s);
	
	if(FindData(ht,N,s))
	{
		printf("find the number you want in database\n");		
	}	
	else 
	{
		printf("no find ,exit program\n");		
	}

	for (i = 0; i < 15; i++)
	{	
		DeleteData(ht,N,keycode[i]);
	}
	
	PrintHashTable(ht,N);



	DestroyHashTable(&ht,N);


	return 0;
}
