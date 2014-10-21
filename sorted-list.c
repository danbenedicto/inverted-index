#include <stdlib.h>
#include <stdio.h>
#include "sorted-list.h"

/*
 * Library of functions to implement a sorted list.
 * Daniel Benedicto and Sarah Bizzarro
 */

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df)
{
	SortedListPtr sl = (SortedListPtr) malloc(sizeof(struct SortedList));
	if (sl == NULL) return NULL;			// malloc check

	sl->head = NULL;
	sl->compare_funct = cf;
	sl->destruct_funct = df;
	
	return sl;
}


void SLDestroy(SortedListPtr list)
{
	LLDestroy(list->head, list->destruct_funct);
	free(list);
}


void LLDestroy(LLNode *head, DestructFuncT destruct_funct)
{
	// recursively destroy list (start with tail)
	if (head == NULL) return;
	LLDestroy(head->next, destruct_funct);
	destruct_funct(head->data);
	free(head);
}


void LLNodeTryDestroy(LLNode *node, DestructFuncT destruct_funct)
{
	if (node->reference_count == 0){
		destruct_funct(node->data);
		free(node);
	} else {
		node->pending_delete = 1;
	}
}


int SLInsert(SortedListPtr list, void *newObj)
{
	LLNode *new_node = (LLNode *) malloc(sizeof(LLNode));
	if (new_node == NULL) return 0;		// malloc check

	new_node->data = newObj;
	new_node->next = NULL;
	new_node->pending_delete = 0;
	new_node->reference_count = 0;

	if (list->head == NULL){
		list->head = new_node;
		return 1;
	}

	int c = list->compare_funct(new_node->data, list->head->data);
	
	if (c < 0){
		// inserted node becomes new head of linked list
		// this could be combined with other if-block? but that seems ugly to me
		new_node->next = list->head;
		list->head = new_node;
		return 1;
	}

	LLNode *prev = NULL;
	LLNode *curr = list->head;	// ptr to traverse list and find insertion point

	while (c > 0){
		prev = curr;
		curr = curr->next;
		if (curr == NULL) break;
		if (curr->pending_delete) continue;
		c = list->compare_funct(new_node->data, curr->data);
	}

	if (c == 0){
		// duplicate value - this is an error condition
		free(new_node);
		return 0;
	}

	// insertion in middle or end
	prev->next = new_node;
	new_node->next = curr;
	return 1;
}


int SLRemove(SortedListPtr list, void *newObj)
{
	if (list->head == NULL) return 0;

	int c = list->compare_funct(list->head->data, newObj);

	if (c == 0){
		// remove and modify head of list
		LLNode *temp = list->head;
		list->head = list->head->next;
		LLNodeTryDestroy(temp, list->destruct_funct);
		return 1;
	}

	LLNode *prev;
	LLNode *curr = list->head;

	while (c != 0){
		prev = curr;
		curr = curr->next;
		if (curr == NULL) break;	// end of list
		if (curr->pending_delete) continue;		// ignore nodes pending deletion
		c = list->compare_funct(curr->data, newObj);
	}

	if (c == 0){
		prev->next = curr->next;
		LLNodeTryDestroy(curr, list->destruct_funct);
		return 1;
	}

	return 0;	// no match in list
}


SortedListIteratorPtr SLCreateIterator(SortedListPtr list)
{
	if (list == NULL){
		printf("error - cannot create iterator for null SortedList\n");
		return NULL;
	}

	SortedListIteratorPtr iter = (SortedListIteratorPtr) malloc(sizeof(struct SortedListIterator));
	if (iter == NULL) return NULL;		// malloc check

	iter->curr = list->head;	// point curr to first node in linked list
	iter->list = list;

	if (iter->curr != NULL){
		iter->curr->reference_count++;
	}

	return iter;
}


void SLDestroyIterator(SortedListIteratorPtr iter)
{
	free(iter);
}


void *SLGetItem(SortedListIteratorPtr iter)
{
	return iter->curr->data;
}

void *SLNextItem(SortedListIteratorPtr iter)
{
	if (iter->curr == NULL) return NULL;

	LLNode *last = iter->curr;
	last->reference_count--;	// soon we won't be pointing at it anymore

	do {
		iter->curr = iter->curr->next;	// advance to next node in linked list (ignore 'removed' nodes)
	} while(iter->curr != NULL && iter->curr->pending_delete);

	if (last->pending_delete){
		// original node was supposed to be deleted, so try to do it now
		LLNodeTryDestroy(last, iter->list->destruct_funct);
	}

	if (iter->curr == NULL) return NULL;

	iter->curr->reference_count++;

	return iter->curr->data;
}