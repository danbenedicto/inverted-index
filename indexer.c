#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "indexer.h"

void index_destroy(Index *index)
{
	if (index == NULL) return;

	trie_destroy(index->root);
	free(index);
}

void trie_destroy(TrieNode *root)
{
	if (root == NULL){
		return;
	}

	trie_destroy(root->first_child);
	trie_destroy(root->next_sibling);

	OccurrenceNode *occ = root->occurrences;
	while (occ != NULL){
		OccurrenceNode *temp = occ;
		occ = occ->next;
		free(temp->file_name);
		free(temp);
	}

	free(root);
}

 
TrieNode *createNode(char letter)
{
	TrieNode *newNode = (TrieNode *)malloc(sizeof(TrieNode));
	newNode->key = tolower(letter);
	newNode->next_sibling = NULL;
	newNode->first_child = NULL;
	newNode->occurrences = NULL;
	newNode->current_occurrences = 0;

	return newNode;
}

Index *create_index()
{
	Index *index = (Index *) malloc(sizeof(Index));
	index->root = createNode('*');

	return index;
}


/*
 * This function takes in a file pointer to the dictionary file and
 * reads and stores words from the Dictionary file into an appropriate data structure.
 */

/*if a prefix of a Dictionary word is in the data file, that’s the prefix count*/
void read_file(Index *index, FILE *file, char *file_name)
{
	char buffer[128];
	int i;

	fscanf(file, "%*[^0-9a-zA-Z]");

	while(fscanf(file, "%[0-9a-zA-Z]%*[^0-9a-zA-Z]", buffer) == 1){
		TrieNode *temp = index->root;
		for(i = 0; buffer[i]; i++){
			temp = get_child(temp, tolower(buffer[i]), 1);
		}
		temp->current_occurrences += 1;
	}

	finalize_file_occurrences(index, file_name);
}

void finalize_file_occurrences(Index *index, char *file_name)
{
	finalize_helper(index->root, file_name);
}

void finalize_helper(TrieNode *node, char *file_name)
{
	if (node == NULL){
		return;
	}

	if (node->current_occurrences > 0){
		OccurrenceNode *occ = create_occurrence(file_name, node->current_occurrences);
		if (node->occurrences == NULL || occ->count > node->occurrences->count){
			occ->next = node->occurrences;
			node->occurrences = occ;
		} else {
			OccurrenceNode *prev = node->occurrences;
			OccurrenceNode *curr = prev->next;
			while (curr != NULL && (occ->count < curr->count)) {
				prev = curr;
				curr = curr->next;
			}
			occ->next = curr;
			prev->next = occ;
		}
	}

	node->current_occurrences = 0;

	finalize_helper(node->first_child, file_name);
	finalize_helper(node->next_sibling, file_name);
}

OccurrenceNode *create_occurrence(char *file_name, int count)
{
	OccurrenceNode *occ = (OccurrenceNode *) malloc(sizeof(OccurrenceNode));
	occ->file_name = (char *) malloc(sizeof(char) * (strlen(file_name) + 1));
	strcpy(occ->file_name, file_name);
	occ->count = count;
	occ->next = NULL;

	return occ;
}


/*
 * Attempts to return a pointer to the parent node's child that contains the letter.
 * If the target node does not exist, it is created if @create is non-zero, otherwise
 * it returns NULL.
 */
TrieNode *get_child(TrieNode *parent, char letter, int create)
{
	TrieNode *prev, *curr, *temp;
	letter = tolower(letter);
	if (parent->first_child == NULL) {
		if (create){
			parent->first_child = createNode(letter);
			return parent->first_child;
		} else {
			return NULL;
		}	
	}

	if (parent->first_child->key > letter){
		if (create){
			temp = parent->first_child;
			parent->first_child = createNode(letter);
			parent->first_child->next_sibling = temp;
			return parent->first_child;
		} else {
			return NULL;
		}
	}

	prev = NULL;
	curr = parent->first_child;

	while (curr != NULL){

		if (curr->key == letter){
			return curr;
		} else if (curr->key > letter){
			if (create){
				prev->next_sibling = createNode(letter);
				prev->next_sibling->next_sibling = curr;
				return prev->next_sibling;
			} else {
				return NULL;
			}
		}
		prev = curr;
		curr = curr->next_sibling;
	}

	if (create){
		prev->next_sibling = createNode(letter);
		return prev->next_sibling;
	} else {
		return NULL;
	}
}

void fprint_index(FILE *out, Index *index)
{
	char buffer[128];
	fprint_index_helper(out, index->root->first_child, buffer, 0);
}

/*
 * Helper function that recursively (VLR) prints the trie.
 * @index is the index of the next empty spot in buffer.
 */
void fprint_index_helper(FILE *out, TrieNode *root, char *buffer, int index)
{
	if (root == NULL) return;

	buffer[index] = root->key;

	if (root->occurrences != NULL){
		buffer[index + 1] = '\0';
		fprintf(out, "<list> %s\n", buffer);
		OccurrenceNode *occ = root->occurrences;
		int count = 0;
		while (occ != NULL){
			fprintf(out, "%s %d ", occ->file_name, occ->count);
			occ = occ->next;
			count++;
			if (count % 5 == 0){
				fprintf(out, "\n");
			}
		}
		if (count % 5 != 0){
			fprintf(out, "\n");
		}
		fprintf(out, "</list>\n");

	}

	fprint_index_helper(out, root->first_child, buffer, index + 1);
	
	buffer[index] = '\0'; /* not really necessary, will get overwritten anyway */

	fprint_index_helper(out, root->next_sibling, buffer, index);
}
