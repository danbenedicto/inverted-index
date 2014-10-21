#ifndef INDEXER_H
#define INDEXER_H

typedef struct {
	char *file_name;
	int occurrences;
} Occurence;

typedef struct {
	char *token;
	SortedListPtr file_occurrences;
} WordIndex;

void scan_file(FILE *file);

void create_node(char *word);

void process_word (char *word);

#endif