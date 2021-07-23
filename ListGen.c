
#include <stdio.h>
#include <stdlib.h>
#include "ListGen.h"
#include "General.h"


BOOL L_init(LIST* pList)
{
	if (pList == NULL) return False;	// no list to initialize

	pList->head.next = NULL;
	return True;
}



NODE* L_insert(LIST* pNode, DATA Value,int(*compare)(const void* a, const void* b))
{
	NODE* tmp;
	NODE* add;
	int flag=1;

	if (!pNode) return NULL;
	add = (NODE*)malloc(sizeof(NODE));
	tmp = (NODE*)malloc(sizeof(NODE));
	
		if(add!=NULL)
		{ 
			if (pNode->head.next == NULL)  //there is no nodes
			{
				add->key = Value;
				add->next = pNode->head.next;
				pNode->head.next = add;
				return add;
			}
			tmp = pNode;
			while(tmp->next!=NULL)
			{
				if (!compare)
				{
					tmp = tmp->next;
					flag = 0;
				}
				else if(compare(tmp->next->key, Value) >= 0)
				{
						add->key = Value;
						add->next = tmp->next;
						tmp->next = add;
						return add;
				}
				if(flag)
						tmp = tmp->next;
				}
			}//add to the last of the list 
			add->key = Value;
			add->next = NULL;
			tmp->next = add;
			return add;
}


BOOL L_delete(NODE* pNode, void(*freeFunc)(void*))
{
	NODE* tmp;

	if (!pNode || !(tmp = pNode->next)) return False;

	pNode->next = tmp->next;
	if (freeFunc != NULL)
		freeFunc(tmp->key);
	free(tmp);
	return True;
}


NODE* L_find(NODE* pNode, DATA value, int(*compare)(const void*, const void*))
{
	NODE* temp = NULL;
	if (!pNode) return NULL;
	while (pNode != NULL)
	{
		if (compare(pNode->key, value) == 0)
		{
			temp = pNode;
			break;
		}
		pNode = pNode->next;
	}

	return temp;


}



BOOL L_free(LIST* pList, void(*freeFunc)(void*))
{
	NODE *tmp;

	if (!pList) return False;
	tmp = &(pList->head);
	BOOL res = True;
	while (res)
	{
		res = L_delete(tmp, freeFunc);
	}

	return True;
}



void L_print(LIST* pList, void(*print)(const void*))
{
	NODE	*tmp;
	if (!pList) return;

	printf("\n");
	for (tmp = pList->head.next; tmp; tmp = tmp->next)
		print(tmp->key);
	printf("\n");
}
void L_printToFile(LIST* pList,FILE* file, void(*print)(const void*,void*))
{
	NODE	*tmp;
	if (!pList) return ;

	printf("\n");
	for (tmp = pList->head.next; tmp; tmp = tmp->next)
		print(tmp->key, file);
	printf("\n");
}
