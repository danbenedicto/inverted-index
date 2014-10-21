all: index

index: main.c trie.c trie.h
	gcc -Wall -g -o index main.c trie.c

clean:
	rm -f index
	rm -fr index.dSYM
