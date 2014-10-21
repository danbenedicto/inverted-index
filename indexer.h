typedef struct OccurrenceStruct {
	char *file_name;
	int count;
	struct OccurrenceStruct *next;
} OccurrenceNode;

typedef struct TrieNodeStruct {
    char key;
    struct TrieNodeStruct *next_sibling;
    struct TrieNodeStruct *first_child;
    OccurrenceNode *occurrences;
    int current_occurrences;
} TrieNode;

typedef struct {
	TrieNode *root;
} Index;

void index_destroy(Index *index);
void trie_destroy(TrieNode *root);
TrieNode *createNode(char letter);
Index *create_index();
void read_file(Index *index, FILE *dict_file, char *file_name);
void finalize_file_occurrences(Index *index, char *file_name);
void finalize_helper(TrieNode *node, char *file_name);
OccurrenceNode *create_occurrence(char *file_name, int count);
TrieNode *get_child(TrieNode *parent, char letter, int create);
void fprint_index(FILE *out, Index *index);
void fprint_index_helper(FILE *out, TrieNode *root, char *buffer, int index);
