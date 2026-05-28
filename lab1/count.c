//
//	File Name: count.c      
//	Author: Rayyan Hussain
//	Date: January 9, 2025
//	Description: This program takes in a text file, reads it, and outputs the total number
//		     of words (strings) in the file
//

#include	<stdio.h>
#define MAX_WORD_LENGTH 30

int main (int argc, char* argv[])
{
	int count = 0;
	char wordSize[MAX_WORD_LENGTH];
	FILE* file = fopen(argv[1], "r");
	// checks if file exists or is valid, if not then error is outputted
	if (file == NULL) {
		perror("file could not be opened");
		return 0;
	}
	// Loops through text file and adds one to count for every string till the last string
	 while (fscanf(file, "%s", wordSize) == 1) {
	 	count++;
	 }
	 printf("%i total words\n", count);
	 fclose(file);
}
