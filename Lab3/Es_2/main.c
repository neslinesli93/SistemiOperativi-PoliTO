#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int i, n, t;

	if (argc < 3)
	{
		fprintf(stdout, "Usage: %s <nProc> <sleepTime>\n", argv[0]);
		exit(1);
	}

	n = atoi(argv[1]);
	t = atoi(argv[2]);

	fprintf(stdout, "Shell:%d, This:%d\n", getppid(), getpid());
	for (i = 0; i < n; i++)
	{
		pid = fork();

		if (pid)
		{
			pid = fork();

			if (pid)
				break;
			
			else if (!pid && i == (n-1))
			{
				sleep(t);
				printf("Process terminated. Father:%d, This:%d\n", getppid(), getpid());
			}
		}

		else if (!pid && i == (n-1))
		{
			sleep(t);
			printf("Process terminated. Father:%d, This:%d\n", getppid(), getpid());
		}
	}

	return 0;
}
