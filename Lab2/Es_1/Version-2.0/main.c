#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "STACK.h"

#define DOT "."
#define DOTDOT ".."

void recursiveVisit(ST stack, char *main_folder);


int main(int argc, char *argv[])
{
	struct stat stat_buf;
	ST stack;
	char *main_folder;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: out <folder>\n");
		exit(1);
	}
	
	main_folder = strdup(argv[1]);
	if (stat(main_folder, &stat_buf) != 0)
	{
		fprintf(stderr, "Error while getting info on the directory\n");
		exit(1);
	}

	if (!S_ISDIR(stat_buf.st_mode))
	{
		fprintf(stderr, "%s is not a directory, exiting\n", argv[1]);
		exit(1);
	}

	stack = STinit();
	STpush(stack, main_folder);

	recursiveVisit(stack, main_folder);

	return 0;
}

void recursiveVisit(ST stack, char *main_folder)
{
	char *fname, *folder;
	int spacing, malloc_size;
	
	struct stat stat_buf, shared_stat_buf;
	DIR *dir_stream;
	struct dirent *dir_entry;

	while (!STempty(stack))
	{
		folder = STpop(stack);
		fprintf(stdout, "\n%s:\n", folder);

		if (stat(folder, &stat_buf) != 0)
		{
			fprintf(stderr, "Error while getting info on the directory\n");
			continue;
		}

		dir_stream = opendir(folder);
		if (dir_stream == NULL)
		{
			fprintf(stdout, "Error while opening the stream\n", folder);
			continue;
		}

		spacing = 0;

		dir_entry = readdir(dir_stream);
		while (dir_entry != NULL)
		{
			// Beautify.jpg
			if (spacing == 4)
			{
				fputc('\n', stdout);
				spacing = 0;
			}

			// malloc_size = <folder> + '/' + <d_name> + '\0'
			malloc_size = (strlen(folder) + strlen(dir_entry->d_name) + 1 + 1);
			fname = malloc(malloc_size * sizeof(char));

			strcpy(fname, folder);
			strcat(fname, "/");
			strcat(fname, dir_entry->d_name);				

			if (stat(fname, &shared_stat_buf) != 0)
			{
				fprintf(stdout, "Generic stat error\n");
				continue;
			}

			if (!S_ISDIR(shared_stat_buf.st_mode))
			{
				spacing++;
				fprintf(stdout, "%s  ", dir_entry->d_name);
				free(fname);
			}
		
			else
			{
				if (!(strcmp(dir_entry->d_name, DOT) == 0 || strcmp(dir_entry->d_name, DOTDOT) == 0))
				{
					spacing++;
					fprintf(stdout, "%s/  ", dir_entry->d_name);
					STpush(stack, fname);
				}
			}

			dir_entry = readdir(dir_stream);
		}
		fputc('\n', stdout);

		if (closedir(dir_stream))
		{
			fprintf(stderr, "Error while closing DIR* stream\n");
			exit(1);
		}

		free(folder);
	}
	
	return;
}
