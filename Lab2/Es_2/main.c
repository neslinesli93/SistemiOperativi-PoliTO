#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <fcntl.h>
#include <unistd.h>

#include <errno.h>

#include "STACK.h"


#define DOT "."
#define DOTDOT ".."
#define BUFSIZE 1024

void recursiveCopy(ST stack, char *src_folder, char *dst_folder);
int isNotDir(char *d);
int copyFile(char *src, char *dst);


int main(int argc, char *argv[])
{
	ST stack;
	char *src_folder, *dst_folder;
	int src_malloc, dst_malloc;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: out <sourceFolder> <destFolder>\n");
		exit(1);
	}

	// This whole malloc stuff is for _adding_ the possible trailing '/'
	src_malloc = strlen(argv[1]);
	src_folder = malloc(sizeof(char) * (src_malloc + 1 + 1));
	if (argv[1][src_malloc - 1] != '/') sprintf(src_folder, "%s/", argv[1]);
	else sprintf(src_folder, "%s", argv[1]);

	dst_malloc = strlen(argv[2]);
	dst_folder = malloc(sizeof(char) * (dst_malloc + 1 + 1));
	if (argv[2][dst_malloc - 1] != '/') sprintf(dst_folder, "%s/", argv[2]);
	else sprintf(dst_folder, "%s", argv[2]);
	
	
	if (isNotDir(src_folder))
	{
		fprintf(stderr, "There was en error with the source folder, exiting\n");
		exit(1);
	}

	if (isNotDir(dst_folder))
	{
		if (mkdir(dst_folder, 0755) != 0)
		{
			fprintf(stderr, "There was en error with the dest folder, exiting\n");
			exit(1);
		}
	}

	stack = STinit();

	// Ugly hack to unify the functions below
	STpush(stack, strdup(""));

	recursiveCopy(stack, src_folder, dst_folder);

	return 0;
}

void recursiveCopy(ST stack, char *src_folder, char *dst_folder)
{
	char *fname, *folder_to_copy, *new_folder, *tmp_folder, *src, *dst, *rel_path;
	int src_malloc_size, dst_malloc_size;
	int src_len = strlen(src_folder), dst_len = strlen(dst_folder), tmp_len, fname_len;
	
	struct stat stat_buf, shared_stat_buf;
	DIR *dir_stream;
	struct dirent *dir_entry;

	// The stack only contains folder _without_ the src/dst directory as a prefix.
	// This is a `do {} while(!STempty());`
	do
	{
		tmp_folder = STpop(stack);
		tmp_len = strlen(tmp_folder);

		// This if/else allocates memory for the paths to be read and to be created;
		// the final paths are complete, not relative.
		// It eventually creates the folder in the new location.
		// N.B: In the allocations, the trailing '/' is NOT included
		dst_malloc_size = dst_len + tmp_len + 1 + 1;
		src_malloc_size = src_len + tmp_len + 1 + 1;

		new_folder = malloc(sizeof(char) * dst_malloc_size);
		sprintf(new_folder, "%s%s", dst_folder, tmp_folder);

		folder_to_copy = malloc(sizeof(char) * src_malloc_size);
		sprintf(folder_to_copy, "%s%s", src_folder, tmp_folder);

		if (mkdir(new_folder, 0755) != 0)
		{
			fprintf(stdout, "Errno creating folder in func#%d\n", errno);
			fprintf(stderr, "Error while creating %s\n", new_folder);

		}

		fprintf(stdout, "\n+Copying %s:\n", folder_to_copy);

		if (stat(folder_to_copy, &stat_buf) != 0)
		{
			fprintf(stderr, "Error while getting info on the directory %s\n", folder_to_copy);
			continue;
		}

		dir_stream = opendir(folder_to_copy);
		if (dir_stream == NULL)
		{
			fprintf(stdout, "Error while opening the stream %s\n", folder_to_copy);
			continue;
		}

		dir_entry = readdir(dir_stream);
		while (dir_entry != NULL)
		{
			fname = strdup(dir_entry->d_name);
			fname_len = strlen(fname);

			dst_malloc_size += fname_len;
			dst = malloc(sizeof(char) * dst_malloc_size);
			sprintf(dst, "%s%s", new_folder, fname);
			
			src_malloc_size += fname_len;
			src = malloc(sizeof(char) * src_malloc_size);
			sprintf(src, "%s%s", folder_to_copy, fname);

			if (stat(src, &shared_stat_buf) != 0)
			{
				fprintf(stdout, "Generic stat error\n");
				continue;
			}

			if (!S_ISDIR(shared_stat_buf.st_mode))
			{
				fprintf(stdout, "%s...", fname);
				if (copyFile(src, dst))
				{
					fprintf(stdout, "Error while copying the file");
				}
				fputc('\n', stdout);
			}
		
			else
			{
				if (!(strcmp(fname, DOT) == 0 ||  strcmp(fname, DOTDOT) == 0))
				{
					fprintf(stdout, "%s->Saving this for later\n", fname);
					rel_path = malloc(sizeof(char) * (tmp_len + fname_len + 1 + 1));
					sprintf(rel_path, "%s%s/", tmp_folder, fname);
					STpush(stack, rel_path);
				}
			}
			dir_entry = readdir(dir_stream);

			free(src);
			free(dst);
			free(fname);
		}
		if (closedir(dir_stream))
		{
			fprintf(stderr, "Error while closing DIR* stream\n");
			exit(1);
		}

		free(new_folder);
		free(folder_to_copy);
		free(tmp_folder);
	}
	while (!STempty(stack));
	
	return;
}

int isNotDir(char *d)
{
	struct stat stat_buf;

	if (stat(d, &stat_buf) != 0)
		return 1;

	if (!S_ISDIR(stat_buf.st_mode))
		return 1;

	return 0;
}


int copyFile(char *src, char *dst)
{
    int fin, fout;
    int nr, nw;
    char buf[BUFSIZE];

	fin = open(src, O_RDONLY);
	if (fin == -1)
	{
		fprintf(stdout, "Error opening src#%d\n", errno);
		fin = open(src, O_RDWR, 0777);
	}

	fout = creat(dst, 0666);
	if (fout == -1)
	{
		fprintf(stdout, "Error opening dst#%d\n", errno);
		fout = creat(dst, 0777);
	}
	
	while ((nr = read (fin, buf, BUFSIZE)) > 0)
	{
		nw = write (fout, buf, nr);
		if (nr != nw)
			fprintf(stdout, "Error write1#%d\n", errno);
	}
	
	if (nw < 0)
		fprintf(stdout, "Error write2#%d\n", errno);

    if (close(fin))
		fprintf(stdout, "Errno close src#%d\n", errno);

    if (close(fout))
		fprintf(stdout, "Errno close dst#%d\n", errno);

	return 0;
}
