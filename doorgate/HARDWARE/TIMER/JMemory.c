/**
* @file JMemory.c
* @brief 管理动态申请内存的模块
*
* @version V0.0.1
* @author joe
* @date 2012年3月27日
* @note
*		none
*
* @copy
*
* 此代码为深圳合杰电子有限公司项目代码，任何人及公司未经许可不得复制传播，或用于
* 本公司以外的项目。本司保留一切追究权利。
*
* <h1><center>&copy; COPYRIGHT 2012 heroje</center></h1>
*/

#include "JMemory.h"
#include <stdlib.h>
#define ALLOCATE_ADDR_TABLE_SIZE  100

static int  p_allocate_addr[ALLOCATE_ADDR_TABLE_SIZE];		//每次动态申请的地址都会保存在此数组里面

void JMemory_init(void)
{
	memset((void*)p_allocate_addr,0,ALLOCATE_ADDR_TABLE_SIZE*sizeof(int));
}

void * Jmalloc(int size)
{
	void *p;
	int		i;

	p = malloc(size);
	if(p)
	{
		for(i = 0;i < ALLOCATE_ADDR_TABLE_SIZE;i++)
		{
			if(p_allocate_addr[i] == 0)
			{
				p_allocate_addr[i] = (int)p;
				break;
			}
		}

		if(i == ALLOCATE_ADDR_TABLE_SIZE)
		{
			p_allocate_addr[0] = (int)p;
		}
	}

	return p;
}


void Jfree(void * p)
{
	int	i;
	for(i = 0;i < ALLOCATE_ADDR_TABLE_SIZE;i++)
	{
		if((int)p == p_allocate_addr[i])
		{
			free(p);
			p_allocate_addr[i] = 0;
			break;
		}
	}
}


void Jfree_all(void)
{
	int	i;
	for(i = 0;i < ALLOCATE_ADDR_TABLE_SIZE;i++)
	{
		if(p_allocate_addr[i])
		{
			free((void*)p_allocate_addr[i]);
		}
	}
}