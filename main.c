#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "indexer.h"

void recursive_read_file(Index *index, char *dir_name)
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
            recursive_read_file(index, path);
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

	if(stat(argv[2], &s) == 0) {
	    if(s.st_mode & S_IFDIR) {
			recursive_read_file(index, (char *) argv[2]);
	    } else if(s.st_mode & S_IFREG) {
	    	FILE *to_invert = fopen(argv[2], "r");
	    	read_file(index, to_invert, (char *) argv[2]);
	    	fclose(to_invert);
	    } else {
	    	printf("error - '%s' is not a file or directory\n", argv[2]);
	    	index_destroy(index);
	    	return -1;
	    }
	} else {
	    printf("error - '%s' is not a file or directory\n", argv[2]);
	    index_destroy(index);
	    return -1;
	}

	char file_name[64];
	strcpy(file_name, argv[1]);
	FILE *inverted = fopen(file_name, "r");

	while (inverted){
		fclose(inverted);
		char choice;
		printf("A file with the name %s already exists. Overwrite? (y/n) ", argv[1]);
		scanf(" %c", &choice);
		if (choice == 'y'){
			break;
		} else {
			printf("Enter a different file name: ");
			scanf("%s", file_name);
			inverted = fopen(file_name, "r");
		}
	}

	inverted = fopen(file_name, "w");
	fprint_index(inverted, index);
	fclose(inverted);

	index_destroy(index);

	return 0;
}
