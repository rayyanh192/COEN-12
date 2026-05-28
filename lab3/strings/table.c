/*
 * File Name: table.c      
 * Author: Rayyan Hussain
 * Last updated: 2/9/25
 * Description: Creates a hash table that stores unique strings using a hash table with linear probing. It allows adding, removing, and retrieving strings while managing memory automatically.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include <string.h>
#include <assert.h>

typedef struct set {
	int length;
	int count;
	char **data;
	char *flags;
} SET;

static int search(SET*, char*elt, bool *found);

/* Big-O: O(1)
 *
 * returns a hash value for a given string using a hash algorithm.
 */
unsigned strhash(char *s) {
	unsigned hash = 0;
	while (*s != '\0') {
		hash = 31 * hash + *s ++;
	}
	return hash;
}

/* Big-O: O(n)
 *
 * creates a new set with a specified maximum capacity, initializes its fields, allocates memory for storing elements, and marks all slots as empty.
*/
SET *createSet(int maxElements) {
	SET *sp = malloc(sizeof(SET));
	assert(sp != NULL);
	sp->compare = compare;
	sp->hash = hash;
	sp->count = 0;
	sp->length = maxElements;
	sp->flags = malloc(sizeof(char)*maxElements);
	assert(sp->flags != NULL);
	sp->data = malloc(sizeof(char*)*maxElements);
	assert(sp->data != NULL);
	for(int i = 0; i < maxElements; i++) {
		sp->flags[i] = 'E';
	}
	return sp;
}

/* Big-O: O(n)
 *
 * deallocates all memory associated with the set, including stored strings, flags, and the set structure itself.
*/
void destroySet(SET *sp) {
	assert(sp != NULL);
	for(int i = 0; i < sp->length; i++) {
		if(sp->flags[i] == 'F') {
			free(sp->data[i]);
		}
	}
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
 * adds a new string to the set if it is not already present, storing a dynamically allocated copy and updating the set’s metadata.
*/
void addElement(SET *sp, char *elt) {
	assert(elt != NULL);
	assert(sp != NULL);
	char *newElement;
	bool found;
	int position = search(sp, elt, &found);
	if(!found) {
		assert(sp->count < sp->length);
		newElement = strdup(elt);
		assert(newElement != NULL);
		sp->data[position] = newElement;
		sp->flags[position] = 'F';
		sp->count++;
	}
}

/* Big-O: O(n)
 *
 * removes a string from the set if it exists, frees its memory, marks its slot as deleted, and updates the element count.
*/
void removeElement(SET *sp, char *elt) {
	assert(sp != NULL);
	assert(elt != NULL);
	bool found;
	int location = search(sp,elt, &found);
	if(found) {
		free(sp->data[location]);
		sp->flags[location] = 'D';
		sp->count--;
	}
}

/* Big-O: O(n)
 *
 * searches for a string in the set and returns a pointer to it if found; otherwise, it returns NULL.
*/
char *findElement(SET *sp, char *elt) {
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
 * allocates and returns an array containing all the stored strings in the set.
*/
char **getElements(SET *sp) {
	assert(sp != NULL);
	char **newData;
	newData = malloc(sizeof(char*)*sp->count);
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
 * searches for a string in the set using linear probing, returning its position if found or the first available slot if not.
*/
static int search(SET *sp, char *elt, bool *found) {
	assert(sp != NULL);
	assert(elt != NULL);
	int hashIndex = (*sp->hash)(elt) % sp->length;
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
		} else if (*sp->compare)(sp->data[position], elt) {
			*found = true;
			return position;
		}
	}
	*found = false;
	return firstDeleted;
}
