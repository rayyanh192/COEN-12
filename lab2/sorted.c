/*
 * File Name: sorted.c
 * Author: Rayyan Hussain
 * Date: January 16, 2025
 * Description: Creates an sorted list and has several functions
	to utilize for the list
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
} SET;

static int search(SET*sp, char*elt, bool *found);

/* Big-O: O(n)
 *
 * initializes and returns a dynamically allocated set with a specified maximum capacity, ensuring memory allocation for both the set structure and its data array.
*/
SET *createSet(int maxElements) {
	SET *sp = malloc(sizeof(SET));
	assert(sp != NULL);
	sp->count = 0;
	sp->length = maxElements;
	sp->data = malloc(sizeof(char*)*maxElements);
	assert(sp->data != NULL);
	return sp;
}

/* Big-O: O(n)
 *
 * deallocates all memory associated with a given set, including its elements, data array, and the set itself.
*/
void destroySet(SET *sp) {
	assert(sp != NULL);
	for(int i = 0; i < sp->count; i++) {
		free(sp->data[i]);
	}
	free(sp->data);
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
 * adds a new element to the set if it does not already exist, ensuring there is sufficient capacity and allocating memory for the new element.
*/
void addElement(SET *sp, char *elt) {
	assert(elt != NULL);
	assert(sp != NULL);
	bool found;
	char *newElement;
	int position = search(sp, elt, &found);
	if(found == false) {
		newElement = strdup(elt);
		assert(newElement != NULL);
		for(int i = sp->count; i>position; i--) {
			sp->data[i] = sp->data[i-1];
		}
		sp->data[position] = newElement;
		sp->count++;
	}
}

/* Big-O: O(n)
 *
 * removes a specified element from the set if it exists, frees its allocated memory, and adjusts the set’s elements to maintain a compact structure.
*/
void removeElement(SET *sp, char *elt) {
	assert(sp != NULL);
	assert(elt != NULL);
	bool found;
	int location = search(sp,elt,&found);
	if(found) {
		free(sp->data[location]);
		for(int i = location+1; i < sp->count; i++) {
			sp->data[i-1] = sp->data[i];
		}
		sp->count--;
	}
}

/* Big-O: O(log(n))
 *
 * searches for a specified element in the set and returns a pointer to it if found, or NULL if the element does not exist.
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
 * creates and returns a new array containing all the elements in the set, preserving their order.
*/
char **getElements(SET *sp) {
	assert(sp != NULL);
	char **newData;
	newData = malloc(sizeof(char*)*sp->length);
	assert(newData != NULL);
	for (int i = 0; i < sp->count; i++) {
		newData[i] = sp->data[i];
	}
	return newData;
}

/* Big-O: O(log(n))
 *
 * utilizes a binary search method to iterate through a set to find the parameter value
*/
static int search(SET *sp, char *elt, bool *found) {
	int low = 0;
	int high = sp->count-1;
	int middle;
	int difference;
	
	while (low <= high) {
		middle = (low+high)/2;
		difference = strcmp(elt, sp->data[middle]);
		if(difference > 0) {
			low = middle + 1;
		} else if (difference < 0) {
			high = middle - 1;
		} else {
			*found = true;
			return middle;
		}
	}
	*found = false;
	return low;
}
