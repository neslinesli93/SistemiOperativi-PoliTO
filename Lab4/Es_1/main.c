#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main ( int argc, char **argv)
{
  int i = 0;

  fprintf (stdout, "Father:%d, This:%d,  %d\n", getppid(), getpid(), ++i);
  execl (argv[0], argv[0], (char *) 0);
  fprintf (stdout, "End program\n");

  return (1);
}
