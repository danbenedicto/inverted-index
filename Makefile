all: index

index: main.c trie.c trie.h
	gcc -Wall -g -o index main.c trie.c

clean:
	rm -f index
	rm -fr index.dSYM

submission:
	tar cfz pa3.tgz main.c trie.c trie.h testplan.txt Makefile