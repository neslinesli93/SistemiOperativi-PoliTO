#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	int len, fd, i, val;
	
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	fd = open(argv[1], O_RDWR);
	if (fd == -1)
	{
		fprintf(stderr, "Error while opening %s", argv[1]);
		exit(-1);
	}

	read(fd, &len, sizeof(int));

	fprintf(stdout, "Not sorted: ");
	for (i = 0; i < len; i++)
	{
		read(fd, &val, sizeof(int));
		fprintf(stdout, "%3d ", val);
	}
	fputc('\n', stdout);

	bubbleFork(len, fd);

	lseek(fd, sizeof(int), SEEK_SET);
	fprintf(stdout, "Sorted: ");
	for (i = 0; i < len; i++)
	{
		read(fd, &val, sizeof(int));
		fprintf(stdout, "%3d ", val);
	}
	fputc('\n', stdout);

	return 0;
}


void bubbleFork(int len, int fd)
{
	int val1, val2, i, j, stat;
	pid_t main_pid, pid;

	for (i = 0; i < len - 1; i++)
	{
		main_pid = fork();

		if (main_pid)
		{
			waitpid(main_pid, &stat, 0);
			exit(0);
		}
		else
		{
			for (j = 0; j < len - 1 - i; j++)
			{
				pid = fork();
				if (pid)
				{
					waitpid(pid, &stat, 0);
					exit(0);
				}
				else
				{
					lseek(fd, (1 + j) * sizeof(int), SEEK_SET);
					read(fd, &val1, sizeof(int));
					read(fd, &val2, sizeof(int));
					
					if (val1 > val2)
					{
						lseek(fd, (1 + j) * sizeof(int), SEEK_SET);
						write(fd, &val2, sizeof(int));
						write(fd, &val1, sizeof(int));
					}
				}
			}
		}
	}

	return;
}
