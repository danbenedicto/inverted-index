all: index

index: main.c trie.c trie.h
	gcc -Wall -g -o index main.c trie.c

clean:
	rm -f index
	rm -fr index.dSYM
	rm -f *.tgz

submission:
	tar cfz pa3.tgz main.c trie.c trie.h testplan.txt Makefile readme.pdf