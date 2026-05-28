/*
 * File Name: list.c      
 * Author: Rayyan Hussain
 * Last updated: 2/20/25
 * Description: This file defines an abstract data type for a deque, implemented using a circular doubly-linked list.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "set.h"
#include <string.h>
#include <assert.h>

struct list {
	int count;
	struct node *head;
	int (*compare)();
};

typedef struct node {
	void *data;
	struct node *next;
	struct node *prev;
} NODE;

/* Big-O: O(1)
 *
 * Allocates and initializes a new circular doubly linked list
 * with a dummy head node.
*/
LIST *createList(int (*compare)()) {
	LIST *lp;
	lp = malloc(sizeof(LIST));
	assert(lp != NULL);
	NODE *head;
	head = malloc(sizeof(NODE));
	assert(head != NULL);
	lp->head = head;
	lp->head->prev = lp->head;
	lp->head->next = lp->head;
	lp->count = 0;
	lp->compare = compare;
	return lp;
}

/* Big-O: O(n)
 *
 * Frees all nodes in the list and
 * deallocates the list structure.
*/
void destroyList(LIST *lp) {
	assert(lp != NULL);
	NODE *np = lp->head;
	while (np->next != np) {
		np->next = np->next->next;
		free(np->next->prev);
		np->next->prev = np;
		lp->count--;
	}
	free(lp->head);
	free(lp);
}

/* Big-O: O(1)
*
* Returns the number of items in
* the list.
*/
int numItems(LIST *lp) {
	assert(lp != NULL);
	return lp->count;
}

/* Big-O: O(1)
 *
 * Adds a new node with the given
 * item at the beginning of the
 * list.
*/
void addFirst(LIST *lp, void *item) {
	assert(lp != NULL && item != NULL);
	NODE *np;
	np = malloc(sizeof(NODE));
	assert(np != NULL);
	np->data = item;
	np->prev = lp->head;
	np->next = lp->head->next;
	lp->head->next->prev = np;
	lp->head->next = np;
	lp->count++;
}

/* Big-O: O(1)
 *
 * Adds a new node with the given
 * item at the end of the list.
*/
void addLast(LIST *lp, void *item) {
	assert(lp != NULL && item != NULL);
	NODE *np;
	np = malloc(sizeof(NODE));
	assert(np != NULL);
	np->data = item;
	np->next = lp->head;
	np->prev = lp->head->prev;
	lp->head->prev->next = np;
	lp->head->prev = np;
	lp->count++;	
}

/* Big-O: O(1)
 *
 * Removes and returns the first
 * item in the list.
*/
void *removeFirst(LIST *lp){
	assert(lp != NULL);
	assert(lp->count > 0);
	//assert(lp->head->next != lp->head);
	NODE *np = lp->head->next;
	void *del = np->data;
	np->next->prev = lp->head;
	lp->head->next = np->next;
	free(np);
	lp->count--;
	return del;
}

/* Big-O: O(1)
 *
 * Removes and returns the last
 * item in the list.
*/
void *removeLast (LIST *lp) {
	assert(lp != NULL);
	NODE * np = lp->head->prev;
    	void *ret = np->data;
	assert (np -> prev != np);
	//np -> prev = np -> prev -> next;
	np->prev->next = np->next;
	lp->head->prev = np->prev;
	free(np);
	lp -> count--;
	return ret;
}

/* Big-O: O(1)
 *
 * Returns the first item in the
 * list without removing it.
*/
void *getFirst (LIST *lp) {
	assert(lp != NULL);
	return lp -> head -> next -> data;
}

/* Big-O: O(1)
 *
 * Returns the last item in the
 * list without removing it.
*/
void *getLast(LIST *lp) {
	assert(lp != NULL);
	return lp -> head -> prev -> data;
}

/* Big-O: O(n)
 *
 * Removes the first occurrence of
 * the given item from the list.
*/
void removeItem (LIST *lp, void *item) {
	assert(lp != NULL && lp->compare != NULL);
	NODE *np = lp->head->next;
	while (np != lp->head) {
		//if(np->next->data == item) {
		if(lp->compare(np->data, item) == 0) {
			np->prev->next = np->next;
			np->next->prev = np->prev;
			free(np);
			lp->count--;
			break;
		}
		np = np->next;
	}
}

/* Big-O: O(n)
 *
 * Searches for the given item in
 * the list and returns it if found.
*/
void *findItem (LIST *lp, void *item) {
	assert(lp != NULL && item != NULL && lp->compare != NULL);
	NODE *np = lp->head->next;
	while (np != lp->head) {
		if((*lp->compare) (np->data, item) == 0) {
			return np->data;
		}
		np = np->next;
	}
	if (np == NULL) {
		return NULL;
	}
	return np->data;
}

/* Big-O: O(n)
 *
 * Creates and returns a new array
 * containing all items in the list
 * in their current order.
*/
void *getItems (LIST *lp) {
	assert(lp != NULL);
	void **items;
	items = malloc(sizeof(void *) * lp -> count);
	assert(items != NULL);
	int i = 0;
	NODE *np = lp -> head -> next;
	while(np != lp -> head) {
		items[i] = np -> data;
		i++;
		np = np->next;
	}
	return items;
}
