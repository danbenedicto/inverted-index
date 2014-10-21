#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorted-list.h"
#include "indexer.h"

/* scans each file in to_invert */

void scan_file(FILE *file){

	char buffer[128];

	fscanf(file, "%*[^0-9a-zA-Z]");

	while(fscanf(file, "%[0-9a-zA-Z]%*[^0-9a-zA-Z]", buffer) == 1){
		process_word(buffer);
	}
}

/*
void create_node(char *word)
{
	IndexNode *newNode = (IndexNode *)malloc(sizeof(IndexNode));
	newNode->name = word;
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->occurrences = NULL;

}
*/

void process_word (char *word)
{
	/* check if word is in LL
	 * if it is in LL, check if file is in its occurence

	 * if file name is not in occurence, add file name to occurrence LL
	 * increment occurence

	 * if it is not LL, add word to word LL
	 * add file name to occruence LL
	 * increment occurence
	 */
	 printf("%s\n", word);
}