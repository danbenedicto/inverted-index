#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "trie.h"

void list_dir(Index *index, char *dir_name)
{
	DIR *dir;
	struct dirent *entry;

	if (!(dir = opendir(dir_name)) || !(entry = readdir(dir))) {
        return;
	}

	do {
		char path[1024];
        int len = snprintf(path, sizeof(path)-1, "%s/%s", dir_name, entry->d_name);
        path[len] = 0;

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0|| strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            list_dir(index, path);
        } else if (entry->d_name[0] != '.') {
            FILE *to_invert = fopen(path, "r");
            read_file(index, to_invert, path);
            fclose(to_invert);
        }
    } while ((entry = readdir(dir)));

    closedir(dir);
}


int main(int argc, char const *argv[])
{
	Index *index = create_index();
	struct stat s;

	if (argc < 3) {
		fprintf(stderr, "invalid input\n");
		return -1;
	}

	if(stat(argv[2], &s) == 0 ) {
	    if(s.st_mode & S_IFDIR) {
			list_dir(index, (char *) argv[2]);
	    } else if(s.st_mode & S_IFREG) {
	    	FILE *to_invert = fopen(argv[2], "r");
	    	read_file(index, to_invert, (char *) argv[2]);
	    	fclose(to_invert);
	    } else {
	    	printf("error\n");
	    }
	}
	else {
	    printf("error\n");
	}

	FILE *inverted = fopen(argv[1], "w");
	fprint_index(inverted, index);
	fclose(inverted);

	index_destroy(index);

	return 0;
}
