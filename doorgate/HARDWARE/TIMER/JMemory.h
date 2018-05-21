#ifndef _JMEMORY_H_
#define _JMEMORY_H_


void JMemory_init(void);
void * Jmalloc(int size);
void Jfree(void * p);
void Jfree_all(void);

#endif