#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define BUF_SIZE 2048
#define END_STRING_LOW "end"
#define END_STRING_UP "END"

void manager(int sig)
{
	if (sig == SIGUSR1)
	{
		return; //PLACEHOLDER
	}
	else if (sig == SIGUSR2)
	{
		return; //PLACEHOLDER
	}
	else if (sig == SIGCHLD)
	{
		return; // PLACEHOLDER
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
	int temp_fd, fdev, fusr, status, dr, dw, len, ready_to_print;
	char *s;
	pid_t dev_pid, usr_pid;

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

	if (signal(SIGCHLD, manager) == SIG_ERR)
	{
		fprintf(stderr, "Signal error");
		exit(1);
	}
	
	temp_fd = open(".tmp_file_must_delete",  0666);
	if (temp_fd == -1)
	{
		fprintf(stderr, "Error while opening tmp file\n");
		exit(-1);
	}
	
	dev_pid = fork();
	if (dev_pid)
	{
		usr_pid = fork();
		if (usr_pid)
		{
			// Father writes its children' pids to files and then waits until
			// its children die, and it dies as well
			fdev = open(".dev_pid", O_CREAT|O_TRUNC|O_WRONLY, 0666);
			write(fdev, &dev_pid, sizeof(int));
			close(fdev);
		
			fusr = open(".usr_pid", O_CREAT|O_TRUNC|O_WRONLY, 0666);;
			write(fusr, &usr_pid, sizeof(int));
			close(fusr);

			sleep(1);

			kill(dev_pid, SIGUSR1);
			kill(usr_pid, SIGUSR2);
			
			wait(&status);
			wait(&status);
			exit(0);
		}
			
		else
		{
			// User is awakened by its father and then it reads developer's pid.
			pause();
			
			usr_pid = getpid();
			fdev = open(".dev_pid", O_RDONLY, 0666);
			read(fdev, &dev_pid, sizeof(int));
			close(fdev);
		}
	}

	else
	{
		// Developer is awakened by its father and then it reads user's pid.
		pause();
		
		dev_pid = getpid();
		fusr = open(".usr_pid", O_RDONLY, 0666);
		read(fusr, &usr_pid, sizeof(int));
		close(fusr);
	}

	s = malloc(BUF_SIZE * sizeof(char));
	
	do
	{
		if (getpid() == dev_pid)
		{
			sleep(1);

			fprintf(stdout, "Enter a string: ");
			scanf("%s", s);
			len = strlen(s) + 1;

			lseek(temp_fd, 0, SEEK_SET);

			dw = write(temp_fd, &len, sizeof(int));
			if (dw == -1)
			{
				fprintf(stderr, "Error while writing to file: %d\n", errno);
				continue;
			}
			
			dw = write(temp_fd, s, len);
			if (dw == -1)
			{
				fprintf(stderr, "Error while writing to file: %d\n", errno);
				continue;
			}
			
			kill(usr_pid, SIGUSR1);
			pause();
		}
		else
		{
			pause();
			ready_to_print = 1;

			lseek(temp_fd, 0, SEEK_SET);
			dr = read(temp_fd, &len, sizeof(int));
			if (dr == -1)
			{
				fprintf(stderr, "Error while reading from file: %d\n", errno);
				ready_to_print = 0;
			}

			dr = read(temp_fd, s, len);
			if (dr == -1)
			{
				fprintf(stderr, "Error while reading from file: %d\n", errno);
				ready_to_print = 0;
			}

			if (ready_to_print)
			{
				string_to_upper(s, len);
				fprintf(stdout, "Converted: %s\n", s);
				fflush(stdout);
			}

			lseek(temp_fd, 0, SEEK_SET);
			kill(dev_pid, SIGUSR2);
		}
	}
	while (strcmp(s, END_STRING_LOW) != 0 && strcmp(s, END_STRING_UP) != 0);

	free(s);

	return 0;
}
