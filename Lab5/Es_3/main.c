#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <signal.h>

void manager(int sig)
{
	if (sig == SIGUSR1)
	{
		fprintf(stdout, "Father Wake-up\n");
	}
	else if (sig == SIGUSR2)
	{
		fprintf(stdout, "Child Wake-up\n");
	}
}

int main(int argc, char *argv[])
{
	pid_t pid;
	
	if (signal(SIGUSR1, manager) == SIG_ERR)
	{
		fprintf(stderr, "Signal error");
		exit(1);
	}
	
	if (signal(SIGUSR2, manager) == SIG_ERR)
	{
		fprintf(stderr, "Signal error");
		exit(1);
	}

	pid = fork();
	while (1)
	{
		if (pid)
		{
			kill(pid, SIGUSR2);
			pause();
		}
		else
		{
			sleep(2);
			kill(getppid(), SIGUSR1);
			pause();
		}
	}

	return 0;
}
