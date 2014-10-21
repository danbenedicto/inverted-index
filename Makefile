all: index

index: main.c indexer.c sorted-list.o
	ar -rv libsl.a sorted-list.o
	gcc -Wall -g -o index main.c indexer.c libsl.a

sorted-list.o: sorted-list.c sorted-list.h
	gcc -Wall -c sorted-list.c

clean:
	rm -f sorted-list.o
	rm -f index
	rm -fr index.dSYM
	rm -f libsl.a
