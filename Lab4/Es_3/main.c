#include <unistd.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>


void fork_right_branch()
{
	pid_t pid;

	fprintf(stdout, "P3: %d\n", getpid());

	pid = fork();
	if (pid)
	{
		fprintf(stdout, "P6: %d\n", getpid());
		while (wait(NULL) != pid);
		fprintf(stdout, "P8: %d\n", getpid());
	}

	exit(0);
}


void fork_left_branch()
{
	pid_t pid;

	fprintf(stdout, "P2: %d\n", getpid());

	pid = fork();
	if (pid)
	{
		fprintf(stdout, "P4: %d\n", getpid());
		while (wait(NULL) != pid);
		fprintf(stdout, "P7: %d\n", getpid());
	}
	else
	{
		fprintf(stdout, "P5: %d\n", getpid());
		exit(0);
	}

	return;
}


int main(int argc, char *argv[])
{
	pid_t main_pid;

	main_pid = fork();
	if (main_pid)
	{
		fprintf(stdout, "P1: %d\n", getpid());

		fork_left_branch();
		while (wait(NULL) != main_pid);

		fprintf(stdout, "P9: %d\n", getpid());
	}
	else
	{
		sleep(2);
		fork_right_branch();
	}

	return 0;
}


