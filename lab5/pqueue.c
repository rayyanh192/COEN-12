/*
 * File Name: pqueue.c      
 * Author: Rayyan Hussain
 * Last updated: 2/27/25
 * Description: This program implements a priority queue using a binary heap, allowing dynamic insertion and removal of elements while maintaining order based on a provided comparison function. It includes functions for queue creation, destruction, element addition, and removal, ensuring efficient heap operations.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pqueue.h"
#define INITIAL_LENGTH 10
static int PARENT(int x) {return (((x)-1)/2);}
static int LEFT(int x) {return ((x)*2+1);}
static int RIGHT(int x) {return ((x)*2+2);}

typedef struct pqueue {
	int count;
	int length;
	void **data;
	int (*compare)();
} PQ;

/*
 * Time Complexity: O(1)
 * Description: initializes and returns a new priority queue with a given comparison function, allocating memory for its structure and data array.
 */
PQ *createQueue(int (*compare)()) {
	PQ *pq;
	pq = malloc(sizeof(PQ));
	assert(pq != NULL);
	pq->data = malloc(sizeof(void*)*INITIAL_LENGTH);
	pq->count = 0;
	pq->length=INITIAL_LENGTH;
	pq->compare = compare;
	return pq;
}

/*
 * Time Complexity: O(1)
 * Description: deallocates memory for the priority queue and its data array.
 */
void destroyQueue(PQ *pq) {
	assert(pq != NULL);
	free(pq->data);
	free(pq);
}

/*
 * Time Complexity: O(1)
 * Description: returns the number of elements currently stored in the priority queue.
 */
int numEntries(PQ *pq) {
	assert(pq != NULL);
	return pq->count;
}

void addEntry(PQ *pq, void *entry) {
	assert(pq != NULL);
	if(pq->count == pq->length) {
		pq->length *= 2;
		pq->data = realloc(pq->data, sizeof(void*)*pq->length);
	}
	pq->data[pq->count] = entry;
	int currIndex = pq->count;
	while ((pq->count != 0) && (pq->compare(pq->data[PARENT(currIndex)],pq->data[currIndex])) > 0) {
		void *temp = pq->data[currIndex];
		pq->data[currIndex] = pq->data[PARENT(currIndex)];
		pq->data[PARENT(currIndex)] = temp;
		currIndex = PARENT(currIndex);
	}
	pq->count++;
}

/*
 * Time Complexity: O(log n)
 * Description: inserts a new element into the priority queue, dynamically resizing the array if needed and maintaining heap order by percolating the element up.
 */
void *removeEntry(PQ *pq) {
	assert (pq != NULL);
	void* root = pq->data[0];
	int index = 0;
	int smallestIndex = 0;
	pq->data[index]=pq->data[pq->count-1];
	pq->count--;
	while (LEFT(index)<pq->count){
		smallestIndex = LEFT(index);
		if (RIGHT(index)<pq->count) {
			if (pq->compare(pq->data[LEFT(index)],pq->data[RIGHT(index)])<0){
				smallestIndex=LEFT(index);
			} else {
				smallestIndex=RIGHT(index);
			}
		}
		if(pq->compare(pq->data[smallestIndex],pq->data[index])<0) {
			void* temp = pq->data[smallestIndex];
			pq->data[smallestIndex] = pq->data[index];
			pq->data[index] = temp;
			index=smallestIndex;
		}
		else {
			break;
		}
	}
	return root;
}
