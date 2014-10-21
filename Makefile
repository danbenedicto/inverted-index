all: index

index: main.c indexer.c indexer.h
	gcc -Wall -g -o index main.c indexer.c

clean:
	rm -f index
	rm -fr index.dSYM
	rm -f *.tgz

submission:
	tar cfz pa3.tgz main.c indexer.c indexer.h testplan.txt Makefile readme.pdf