

#ifndef _LIST_
#define _LIST_

#include "General.h"


typedef struct node
{
	DATA			key;
	struct node*	next;
}NODE;

typedef struct
{
	NODE head;
}LIST;


void L_printToFile(LIST* pList, FILE* file, void(*print)(const void*,void*));

BOOL L_init(LIST* pList);					// create new list

NODE* L_insert(LIST* pNode, DATA Value, int (*compare)(const void* a, const void* b));	// add new node after *pNode

BOOL L_delete(NODE* pNode, void(*freeFunc)(void*));					// erase node after *pNode

NODE* L_find(NODE* pNode, DATA Value, int(*compare)(const void*, const void*));		// return a pointer to the node 

BOOL L_free(LIST* pList, void(*freeFunc)(void*));					// free list memory

void L_print(LIST* pList, void(*print)(const void*));					// print the list content

#endif
#pragma once
