#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char flag;
	int i;

	if (argc < 2)
	{
		fprintf(stdout, "Usage: %s <character>\n", argv[0]);
		exit(1);
	}

	flag = argv[1][0];
	if (flag == 'a')
	{
		printf("Shell:%d, This: %d\n", getppid(), getpid());
		for (i=1; i<=2; i++) {
			if (!fork ())
				printf("Process #%d: %d\n", getpid(), i);
		}
		printf("Process #%d: %d\n", getpid(), i);
	}
	
	else if (flag == 'b')
	{
		printf("Shell:%d, This: %d\n", getppid(), getpid());
		for(i=3; i>1; i--) {
			if (fork ())
				printf("Process #%d: %d\n", getpid(), i);
		}
		printf("Process #%d: %d\n", getpid(), i);
	}
	
	else if (flag == 'c')
	{
		printf("Shell:%d, This: %d\n", getppid(), getpid());
		for (i=0; i<2; i++) {
			if (fork ())
				fork ();
		}
		printf("Process #%d: %d\n", getpid(), i);
	}
	
	else if (flag == 'd')
	{
		printf("Shell:%d, This: %d\n", getppid(), getpid());
		for (i=2; i>=1; i--) {
			if (!fork ())
				printf("Process #%d: %d\n", getpid(), -i);
			else
				printf("Process #%d: %d\n", getpid(), i);
		}
	}

	return 0;
}
