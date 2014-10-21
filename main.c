#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include "trie.h"

void list_dir(const char *dir_name){

	DIR *dir;
	dir = opendir(dir_name);

	if (!dir) {
		printf("no dir\n");
	}

	while (1) {

		struct dirent *entry;
		const char *d_name;

		entry = readdir(dir);
		if (!entry) {
			break;
		}

		d_name = entry->d_name;

		if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {

			FILE *to_invert = fopen(d_name, "r");
	    	read_file(index, to_invert, d_name);


			printf ("%s/%s\n", dir_name, d_name);

		}

		if (entry->d_type & DT_DIR) {
        
	        if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {

	            int path_length;
	            char path[PATH_MAX];

	            path_length = snprintf (path, PATH_MAX, "%s/%s", dir_name, d_name);
	            printf ("%s\n", path);

				list_dir (path);
	        }
		}
    }
}


int main(int argc, char const *argv[]){

	Index *index = create_index();
	struct stat s;
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

	if(stat(argv[2],&s) == 0 ) {

	    if(s.st_mode & S_IFDIR) {

			list_dir (argv[2]);

	    }
	    else if(s.st_mode & S_IFREG) {

	    	to_invert = fopen(argv[2], "r");
	    	read_file(index, to_invert, (char *) argv[2]);
	    	index_destroy(index);

	    }
	    else {
	    	printf("error\n");
	    }
	}
	else {

	    printf("error\n");
	}

	return 0;
	//scan_file(to_invert);

	/* parse file and read each text file until end of this .txt file */

}
