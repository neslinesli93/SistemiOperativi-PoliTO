#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 2048

int main(int argc, char *argv[])
{
	FILE *fp;
	char **comm, *s, **exec_args, *token;
	int i, j, flag, line, tmp_len, status;
	int max_lines = 100;

	if (argc < 3)
	{
		fprintf(stdout, "Usage: %s <commands.txt> <flag>\n\n", argv[0]);
		fprintf(stdout, "Flag can be 0 (use system) or 1 (use exec*)\n");
		exit(1);
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		fprintf(stdout, "Error while opening %s. Exiting\n", argv[1]);
		exit(1);
	}

	flag = atoi(argv[2]);
	if (flag < 0 || flag > 1)
	{
		fprintf(stdout, "Incorrect flag,run the program with no arguments for help\n");
		exit(1);
	}
	
	// s represents a single command, and it can contains up to BUF_SIZE bytes
	// comm is an array of those commands, and the program can reallocate it if necessary
	s = malloc(sizeof(char) * BUF_SIZE);
	comm = malloc(sizeof(char *) * max_lines);
	for (i = 0; i < max_lines; i++)
		comm[i] = malloc(sizeof(char) * BUF_SIZE);

	line = 0;
	while (fgets(s, BUF_SIZE - 1, fp) != NULL)
	{
		tmp_len = strlen(s);

		// Each string should end with " end\n", so 5 characters
		strncpy(comm[line], s, tmp_len - 5);
		line++;

		// If there are more lines than expected, realloc the array of commands
		if (line >= max_lines)
		{
			max_lines = max_lines * 2;
			comm = realloc(comm, max_lines);
			for (i = line; i < max_lines; i++)
				comm[i] = malloc(sizeof(char) * BUF_SIZE);
		}
	}

	// Here starts the real command processing
	// system implementation is straight-forward
	if (flag == 0)
	{
		for (i = 0; i < line; i++)
		{
			system(comm[i]);
		}
	}

	// exec is a bit more tricky, since I have to tokenize all the commands and
	// their params. Thank you, cstring.h
	// http://www.cplusplus.com/reference/cstring/strtok/
	else if (flag == 1)
	{
		exec_args = malloc(sizeof(char *)  * BUF_SIZE);
		for (i = 0; i < line; i++)
		{
			j = 0;
			token = strtok (comm[i], " ");
			while (token != NULL)
			{
				exec_args[j] = token;
				token = strtok(NULL, " ");
				j++;
			}
			exec_args[j] = (char *) 0;

			if (fork())
			{
				wait(&status);
			}
			else
			{
				sleep(3);

				execvp(exec_args[0], exec_args);
				fprintf(stdout, "Exec error: %d\n", errno);
				exit(0);
			}
		}

		free(exec_args);
	}

	free(s);
	for (i = 0; i < max_lines; i++)
		free(comm[i]);
	free(comm);

	
	return 0;
}
