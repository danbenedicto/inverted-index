#include <stdlib.h>
#include <stdio.h>
#include "sorted-list.h"
#include "indexer.h"

int main(int argc, char const *argv[]){

	FILE *to_invert, *inverted;

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
	scan_file(to_invert);

	/* parse file and read each text file until end of this .txt file */

}