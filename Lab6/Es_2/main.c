#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#define BUF_SIZE 1024
#define NORM_SEQ "?"
#define STOP_SEQ "!"
#define SEQ_LEN 2

int main(int argc, char *argv[])
{
	int pipe_father[2], pipe_child[2], len, max_iter, i;
	char *c;
	pid_t pid;


	if (argc < 2)
	{
		fprintf(stdout, "Usage: %s <MAX_ITERATIONS>\n", argv[0]);
		exit(1);
	}

	max_iter = atoi(argv[1]);
	c = malloc(sizeof(char) * SEQ_LEN);

	if (pipe(pipe_father) || pipe(pipe_child))
	{
		fprintf(stderr, "Error while creating one of the pipes\n");
		exit(-1);
	}

	// Each process reads from the pipe with its own name, and writes
	// to the pipe with its sibling name
	pid = fork();
	if (pid)
	{
		close(pipe_father[1]);
		close(pipe_child[0]);
	}
	else
	{
		close(pipe_child[1]);
		close(pipe_father[0]);
	}

	for(i = 0; ; i++)
	{
		if (pid)
		{
			if (i != max_iter)
			{
				fprintf(stdout, "I am the father: %d\n", getpid());
				write(pipe_child[1], NORM_SEQ, sizeof(char) * SEQ_LEN);
				read(pipe_father[0], c, sizeof(char) * SEQ_LEN);
			}
			else
			{
				fprintf(stdout, "Father: STOP\n");
				write(pipe_child[1], STOP_SEQ, sizeof(char) * SEQ_LEN);
				break;
			}
		}

		else
		{
			read(pipe_child[0], c, sizeof(char) * SEQ_LEN);
			if (strcmp(c, NORM_SEQ) == 0)
			{
				fprintf(stdout, "I am the child: %d\n", getpid());
				write(pipe_father[1], " ", sizeof(char) * SEQ_LEN);
			}
			else
			{
				fprintf(stdout, "Child: OK\n");
				break;
			}
		}
	}

	return 0;
}
