/*
 * File Name: set.c
 * Author: Rayyan Hussain
 * Last updated: 2/20/25
 * Description: Creates a hash table and several functions to perform with
 * the table, utilizing an array of lists.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "set.h"
#include <string.h>
#include <assert.h>
#include "list.h"

typedef struct set {
    int count;
    int length;
    LIST **list;
    int (*compare)();
    unsigned (*hash)();
} SET;

/* Big-O: O(n)
 *
 * Initializes and returns a hash
 * table implemented as an array
 * of lists, ensuring memory
 * allocation for all components.
*/
SET *createSet(int maxElements, int (*compare)(), unsigned (*hash)()) {
    SET *sp = malloc(sizeof(SET));
    assert(sp != NULL);
    sp->count = 0;
    sp->length = maxElements/20;
    sp->compare = compare;
    sp->hash = hash;
    sp->list = malloc(sizeof(LIST*)*sp->length);
    assert(sp->list != NULL);
    for (int i = 0; i < sp->length; i++) {
        sp->list[i] = createList(compare);
    }
    return sp;
}

/* Big-O: O(n)
 *
 * Deallocates all memory for the
 * hash table, including all lists
 * and the table structure itself.
*/
void destroySet(SET *sp) {
    assert(sp != NULL);
    for (int i = 0; i < sp->length; i++) {
        destroyList(sp->list[i]);
    }
    free(sp->list);
    free(sp);
}

/* Big-O: O(1)
 *
 * Returns the number of elements
 * currently stored in the set.
*/
int numElements(SET *sp) {
    assert(sp != NULL);
    return (sp->count);
}

/* Big-O: O(n)
 *
 * Adds an element to the hash
 * table if it does not already
 * exist, inserting it into the
 * appropriate list.
*/
void addElement(SET *sp, void *elt) {
    assert(elt != NULL);
    assert(sp != NULL);
    int index = (*sp->hash)(elt) % sp->length;
    void *data = findItem(sp->list[index], elt);
    if (data == NULL) {
        addFirst(sp->list[index], elt);
        sp->count++;
    }
}

/* Big-O: O(n)
 *
 * Removes an element from the
 * hash table if it exists,
 * deleting it from the correct
 * list.
*/
void removeElement(SET *sp, void *elt)
{
    assert(sp!= NULL);
    assert(elt != NULL);
    int index = (*sp -> hash)(elt) % sp->length;
    void *data = findItem(sp->list[index], elt);
    if(data != NULL) {
        removeItem(sp -> list[index] ,elt);
        sp -> count--;
    }
}

/* Big-O: O(n)
 *
 * Searches for an element in the
 * hash table and returns it if
 * found, otherwise returns NULL.
*/
void *findElement (SET *sp, void *elt) {
    assert(sp != NULL);
    assert(elt != NULL);
       int index = (*sp -> hash)(elt) % sp->length;
       return findItem(sp->list[index],elt);
}

/* Big-O: O(n)
 *
 * Creates and returns an array
 * containing all elements in the
 * hash table, preserving order.
*/
void *getElements(SET *sp)
{
    assert(sp != NULL);
    void **dataCopy = malloc(sizeof(void *)*sp->count);
    assert(dataCopy != NULL);
    void **dataInsert;
    int n;
    int index = 0;
    for(int i=0;i<sp->length;i++){
        dataInsert = getItems(sp->list[i]);
        n = numItems(sp->list[i]);
        memcpy(dataCopy+index,dataInsert,n*sizeof(void*));
        index += n;
    }
    return dataCopy;
}
