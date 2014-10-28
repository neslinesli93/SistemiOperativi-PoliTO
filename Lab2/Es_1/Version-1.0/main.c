#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define DOT "."
#define DOTDOT ".."

void recursiveVisit(char *, int);

int main(int argc, char *argv[])
{
	struct stat stat_buf;

	if (stat(argv[1], &stat_buf) != 0)
	{
		fprintf(stderr, "Error while getting info on the directory\n");
		exit(1);
	}

	if (!S_ISDIR(stat_buf.st_mode))
	{
		fprintf(stderr, "%s is not a directory, exiting\n", argv[1]);
		exit(1);
	}


	recursiveVisit(argv[1], 1);

	return 0;
}

void recursiveVisit(char *folder, int level)
{
	char *fname;
	int i, malloc_size;
	
	struct stat stat_buf, temp_stat_buf;
	mode_t stat_mode;

	DIR *dir_stream;
	struct dirent *dir_entry;

	for (i = 0; i < level-1; i++) fputc(' ', stdout);
	fprintf(stdout, "+%s\n", folder);

	if (stat(folder, &stat_buf) != 0)
	{
		fprintf(stdout, "Error while getting info on %s\n", folder);
		return;
	}

	dir_stream = opendir(folder);
	if (dir_stream == NULL)
	{
		fprintf(stdout, "Error while opening stream of %s\n", folder);
		return;
	}

	dir_entry = readdir(dir_stream);
	while (dir_entry != NULL)
	{
		// malloc_size = <folder> + '/' + <d_name> + '\0'
		malloc_size = (strlen(folder) + strlen(dir_entry->d_name) + 1 + 1);
		fname = malloc(malloc_size * sizeof(char));
		strcpy(fname, folder);
		strcat(fname, "/");
		strcat(fname, dir_entry->d_name);				
		
		if (stat(fname, &temp_stat_buf) != 0)
		{
			fprintf(stdout, "Generic stat error?");
			return;
		}

		if (!S_ISDIR(temp_stat_buf.st_mode))
		{
			for (i = 0; i < level; i++) fputc(' ', stdout);
			fprintf(stdout, "%s\n", dir_entry->d_name);
		}
		
		else
		{
			if (!(strcmp(dir_entry->d_name, DOT) == 0 || strcmp(dir_entry->d_name, DOTDOT) == 0))
			{
				recursiveVisit(fname, level + 1);
			}
		}

		free(fname);
		dir_entry = readdir(dir_stream);
	}

	if (closedir(dir_stream))
	{
		fprintf(stderr, "Error while closing DIR* stream\n");
		exit(1);
	}

	for (i = 0; i < level-1; i++) fputc(' ', stdout);
	fprintf(stdout, "-%s\n\n", folder);
	
	return;
}
