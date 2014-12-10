#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
#define END_STRING_LOW "end"
#define END_STRING_UP "END"


void string_to_upper(char *s, int len)
{
	int i;

	for (i = 0; i < len; i++)
	{
		if (s[i] > 96 && s[i] < 123)
		{
			s[i] -= 32;
		}
	}
}


int main(int argc, char *argv[])
{
	int main_pipe[2], status_pipe[2], status, len;
	pid_t dev_pid = 0, usr_pid = 0;
	char *s, *c;

	if (pipe(main_pipe) || pipe(status_pipe))
	{
		fprintf(stderr, "Error with the pipe\n");
		exit(-1);
	}

	s = malloc(sizeof(char) * BUF_SIZE);
	c = malloc(sizeof(char));

	if (fork())
	{
		// Father waits till its children die
		if (fork())
		{
			wait(&status);
			wait(&status);
			exit(0);
		}

		// usr process does not need to write to main_pipe, nor to read from status_pipe
		else
		{
			usr_pid = getpid();
			close(main_pipe[1]);
			close(status_pipe[0]);
		}
	}

	// dev process does not need to read from main_pipe, nor to write to status_pipe
	else
	{
		dev_pid = getpid();
		close(main_pipe[0]);
		close(status_pipe[1]);
	}

	// I need a way to stop both the processes from reading/writing without sync.
	// This is achieved with two pipes: one for string manipulation, one for "status"
	// report, which blocks father from writing with no limits
	do
	{
		if (getpid() == dev_pid)
		{
			fprintf(stdout, "Enter a string: ");
			scanf("%s", s);
			len = strlen(s) + 1;

			write(main_pipe[1], &len, sizeof(int));
			write(main_pipe[1], s, sizeof(char) * len);

			read(status_pipe[0], c, sizeof(char));
		}
		else
		{
			read(main_pipe[0], &len, sizeof(int));
			read(main_pipe[0], s, sizeof(char) * len);

			string_to_upper(s, len);
			fprintf(stdout, "Converted: %s\n", s);
			fflush(stdout);

			write(status_pipe[1], c, sizeof(char));
		}
	}
	while (strcmp(s, END_STRING_LOW) != 0 && strcmp(s, END_STRING_UP) != 0);

	free(s);
	free(c);

	return 0;
}
