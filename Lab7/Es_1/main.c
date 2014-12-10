#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>


int main (int argc, char *argv[])
{
	int *v, n, i, j;
	pid_t pid1, pid2;

	if (argc < 2)
	{
		fprintf(stdout, "Usage: %s <n>\n", argv[0]);
		exit(1);
	}

	n = atoi(argv[1]);
	v = (int *)malloc(n * sizeof(int));

	// Two processes are created after every iteration.
	// Each one of them gets a copy of the array as it was stored inside
	// its father's heap. So the idea is to create 2^n processes which all have
	// a different array(that is a different n-bit binary number) to print.
	// One of them gets a 0, the other gets a 1
	for (i = 0; i < n; i++)
	{
		if (fork())
		{
			if (fork())
			{
				wait((int *) 0);
				wait((int *) 0);
				exit(0);
			}
			else
			{
				v[i] = 0;
			}
		}
		else
			v[i] = 1;
	}

	for (j = 0; j < n; j++)
	{
		fprintf(stdout, "%d", v[j]);
	}
	fprintf(stdout, "\n");
	
	return 0;
}

