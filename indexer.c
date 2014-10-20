#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct OccurrStruct {
	char *file_name;
	int occurrences
	struct OccurrStruct next;
	struct OccurrStruct prev;
} OccurrNode;

struct WordStruct {
	char *name;
	struct WordStruct next;
	struct WordStruct prev;
	struct OccurrStruct occurrence_name;
} WordNode;


int main(int argc, char const *argv[]){

	FILE *to_invert

	if (argc < 3) {
		fprintf(stderr, "invalid input\n");
		return -1;
	}

	inverted = fopen(argv[1], "w");

	/* If a directory, you need to
	 * recursively index all files in the directory (and its sub-directories). If a file, you just
	 * need to index that single file.
	 */

	to_invert = fopen(argv[2], "r");

	/* parse file and read each text file until end of this .txt file *//

}

/* scans each file in to_invert */

void scan_file(FILE *file){

	char buffer[128];

	fscanf(data_file, "%*[^0-9a-zA-Z]");

	while(fscanf(data_file, "%[0-9a-zA-Z]%*[^0-9a-zA-Z]", buffer) == 1){

		process_word(buffer);

	}
}

void create_node(char *word){

	IndexNode *newNode = (IndexNode *)malloc(sizeof(IndexNode));
	newNode->name = word;
	newNode->next = NULL;
	newNode->prev = NULL;
	newNode->occurrences = NULL;

}

void process_word (char *word){

	/* check if word is in LL
	 * if it is in LL, check if file is in its occurence

	 * if file name is not in occurence, add file name to occurrence LL
	 * increment occurence

	 * if it is not LL, add word to word LL
	 * add file name to occruence LL
	 * increment occurence
	 */



}