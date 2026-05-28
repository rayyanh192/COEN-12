/*
 * File Name: table.c      
 * Author: Rayyan Hussain
 * Last updated: 2/9/25
 * Description: creates a generic set ADT that can store any data type using void pointers. It allows custom comparison and hashing functions, making it flexible while requiring the user to manage memory.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include <string.h>
#include <assert.h>

typedef struct set {
	int count;
	int length;
	void **data;
	char *flags;
	int (*compare)();
	unsigned (*hash)();
} SET;

static int search(SET*, void*elt, bool *found);

/* Big-O: O(n)
 *
 * creates a new generic set with a specified capacity, initializes its fields, and stores function pointers for comparison and hashing.
*/
SET *createSet(int maxElements, int (*compare)(), unsigned (*hash)()) {
	SET *sp = malloc(sizeof(SET));
	assert(sp != NULL);
	sp->count = 0;
	sp->length = maxElements;
	sp->compare = compare;
	sp->hash = hash;
	sp->flags = malloc(sizeof(char)*maxElements);
	assert(sp->flags != NULL);
	sp->data = malloc(sizeof(void*)*maxElements);
	assert(sp->data != NULL);
	for(int i = 0; i < maxElements; i++) {
		sp->flags[i] = 'E';
	}
	return sp;
}

/* Big-O: O(1)
 *
 * deallocates all memory associated with the generic set, including stored data pointers, flags, and the set structure itself.
*/
void destroySet(SET *sp) {
	assert(sp != NULL);
	free(sp->data);
	free(sp->flags);
	free(sp);
}

/* Big-O: O(1)
 *
 * returns the current number of elements in the given set.
*/
int numElements(SET *sp) {
	assert(sp != NULL);
	return (sp->count);
}

/* Big-O: O(n)
 *
 * adds a new element to the generic set if it is not already present, storing its pointer and updating the set’s metadata.
*/
void addElement(SET *sp, void *elt) {
	assert(elt != NULL);
	assert(sp != NULL);
	void *newElement;
	bool found;
	int position = search(sp, elt, &found);
	if(!found) {
		assert(sp->count < sp->length);
		newElement = elt;
		assert(newElement != NULL);
		sp->data[position] = newElement;
		sp->flags[position] = 'F';
		sp->count++;
	}
}

/* Big-O: O(n)
 *
 * removes an element from the generic set if it exists by marking its slot as deleted and updating the element count.
*/
void removeElement(SET *sp, void *elt) {
	assert(sp != NULL);
	assert(elt != NULL);
	bool found;
	int location = search(sp,elt, &found);
	if(found) {
		//free(sp->data[location]);
		sp->flags[location] = 'D';
		sp->count--;
	}
}

/* Big-O: O(n)
 *
 * searches for an element in the generic set and returns a pointer to it if found; otherwise, it returns NULL.
*/
void *findElement(SET *sp, void *elt) {
	assert(sp != NULL);
	assert(elt != NULL);
	bool found;
	int location = search(sp, elt, &found);
	if(!found) {
		return NULL;
	}
	return sp->data[location];
}

/* Big-O: O(n)
 *
 * allocates and returns an array containing all stored elements in the generic set.
*/
void *getElements(SET *sp) {
	assert(sp != NULL);
	void **newData;
	newData = malloc(sizeof(void*)*sp->count);
	assert(newData != NULL);
	int j = 0;
	for (int i = 0; i < sp->length; i++, j++) {
		if(sp->flags[i]!='F') {
			j--;
			continue;
		}
		newData[j] = sp->data[i];
	}
	return newData;
}

/* Big-O: O(n)
 *
 * searches for an element in the generic set using linear probing, returning its position if found or the first available slot if not.
*/
static int search(SET *sp, void *elt, bool *found) {
	assert(sp != NULL);
	assert(elt != NULL);
	int hashIndex = (sp->hash)(elt) % sp->length;
	int firstDeleted = -1;
	int position;
	
	for (int i = 0; i < sp->length; i++) {
		position = (hashIndex + i) % sp->length;
		if (sp->flags[position] == 'D') {
			if (firstDeleted == -1) {
				firstDeleted = position;
			} 
		} else if (sp->flags[position] == 'E') {
			*found = false;
			if (firstDeleted == -1) {
				return position;
			} else {
				return firstDeleted;
			}
		} else if ((*sp->compare)(sp->data[position], elt)==0) {
			*found = true;
			return position;
		}
	}
	*found = false;
	return firstDeleted;
}
