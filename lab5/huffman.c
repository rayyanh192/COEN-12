/*
 * File Name: pqueue.c      
 * Author: Rayyan Hussain
 * Last updated: 3/6/25
 * Description: implements Huffman encoding by reading character frequencies from a file, constructing a Huffman tree using a priority queue, and computing the depth of each character in the tree. It then prints the character frequencies and bit allocations before calling the pack function to compress the file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "pqueue.h"
#include "pack.h"

typedef struct node NODE;
int count[257];
NODE* nodes[257];

/*
 * Time Complexity: O(1)
 * Description: Compares two nodes based on their frequency counts and returns -1, 1, or 0 accordingly.
 */
static int compare(NODE *t1, NODE *t2)
{
    assert(t1 != NULL);
    assert(t2 != NULL);
    return ((t1 -> count < t2 -> count) ? -1 : (t1 -> count > t2 -> count));
}

/*
 * Time Complexity: O(1)
 * Description: Allocates memory for a new node, initializes its frequency count, and assigns parent pointers to its children.
 */
static NODE* mknode(int data, NODE *left_node, NODE *right_node)
{
    NODE* new_node = malloc(sizeof(NODE));
    assert(new_node != NULL);
    new_node -> count = data;
    new_node -> parent = NULL;
    if (left_node != NULL)
    {
        left_node -> parent = new_node;
    }
    if (right_node != NULL)
    {
        right_node -> parent = new_node;
    }

    return new_node;
}

/*
 * Time Complexity: O(h)
 * Description: Recursively computes the depth of a given node by traversing up to its parent.
 */
static int depth(NODE* node)
{
    assert(node != NULL);
    if(node -> parent == NULL)
    {
        return 0;
    }
    return depth(node -> parent) + 1;
}

/*
 * Time Complexity: O(n log n)
 * Description: Reads character frequencies from a file, builds a Huffman tree using a priority queue, calculates bit allocations, and compresses the file.
 */
int main(int argc, char *argv[]) {
    if(argc == 2) {
        printf("Cannot open file\n");
        return 0;
    }
    else {
        FILE* infp = fopen(argv[1], "r");
        if(infp == NULL)
        {
            printf("File Error\n");
            return;
        }

        int ch = getc(infp);
        while(ch != EOF)
        {
            count[ch]++;
            ch = getc(infp);
        }

        fclose(infp);

        return;
    }

    PQ* pq = createQueue(compare);
    int i;
    for (i = 0; i < 256; i++) {
        if (count[i] > 0)
        {
            nodes[i] = mknode(count[i], NULL, NULL);
            addEntry(pq, nodes[i]);
        }
    }
    
    nodes[256] = mknode(0, NULL, NULL);
    addEntry(pq, nodes[256]);

    while(numEntries(pq) > 1) {
    	NODE* left = removeEntry(pq);
        NODE* right = removeEntry(pq);
        NODE* parent = mknode(left -> count + right -> count, left, right);
        addEntry(pq, parent);
    }

    for (int j = 0; j < 256; j++) {
        if (nodes[j] != NULL)
        {
            printf(isprint(j) ? "%c" : "%03o", j);
            printf(": %d x %d bits = %d bits\n", count[j], depth(nodes[j]), count[j] * depth(nodes[j]));
        }
    }

    pack(argv[1], argv[2], nodes);
    destroyQueue(pq);

    return 0;
}
