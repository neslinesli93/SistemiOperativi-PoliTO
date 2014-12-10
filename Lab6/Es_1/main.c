#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 1024
#define END_STRING_LOW "end"
#define END_STRING_UP "END"

void manager(int sig)
{
	if (sig == SIGUSR1)
	{
		return; //PLACEHOLDER
	}
}

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
	int fd[2], status, len;
	pid_t dev_pid = 0, usr_pid = 0;
	char *s;

	if (signal(SIGUSR1, manager) == SIG_ERR)
	{
		fprintf(stderr, "Signal error\n");
		exit(1);
	}

	if (pipe(fd))
	{
		fprintf(stderr, "Error with the pipe\n");
		exit(-1);
	}

	s = malloc(sizeof(char) * BUF_SIZE);
	
	dev_pid = fork();
	if (dev_pid)
	{
		usr_pid = fork();
		if (usr_pid)
		{
			write(fd[1], &dev_pid, sizeof(pid_t));

			wait(&status);
			wait(&status);
			exit(0);
		}

		// usr process does not need to write to pipe
		else
		{
			close(fd[1]);
			read(fd[0], &dev_pid, sizeof(pid_t));
		}
	}

	// dev process does not need to read from pipe
	else
	{
		dev_pid = getpid();
		close(fd[0]);
	}

	// I need a way to stop both the processes from reading/writing without sync.
	// As for the usr process, this is done automatically by the read(), since
	// it is blocking; as for the dev process, I achieve that with signals

	// N.B: See Es_2 for a nice way to avoid pause and signals: two pipes!
	do
	{
		if (getpid() == dev_pid)
		{
			fprintf(stdout, "Enter a string: ");
			scanf("%s", s);
			len = strlen(s) + 1;

			write(fd[1], &len, sizeof(int));
			write(fd[1], s, sizeof(char) * len);

			pause();
		}
		else
		{
			sleep(1);

			read(fd[0], &len, sizeof(int));
			read(fd[0], s, sizeof(char) * len);

			string_to_upper(s, len);
			fprintf(stdout, "Converted: %s\n", s);
			fflush(stdout);

			kill(dev_pid, SIGUSR1);
		}
	}
	while (strcmp(s, END_STRING_LOW) != 0 && strcmp(s, END_STRING_UP) != 0);

	return 0;
}
