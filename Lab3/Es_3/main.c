#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	int n, temp_d, *v, i, statVal;
	pid_t pid;
	
	if (argc < 2)
	{
		fprintf(stdout, "Usage: %s <nElem>\n", argv[0]);
		exit(1);
	}

	n = atoi(argv[1]);
	v = malloc(sizeof(int) * n);

	fprintf(stdout, "Insert %d numbers:\n", n);
	for (i = 0; i < n; i++)
	{
		scanf("%d", &temp_d);
		v[i] = temp_d;
	}

	fprintf(stdout, "\nPrint those numbers in reverse order:\n");
	for (i = 0; i < n; i++)
	{
		pid = fork();
		if (pid)
		{
			waitpid(pid, &statVal, 0);
			fprintf(stdout, "%d\n", v[i]);
			break;
		}
	}

	return 0;
}
