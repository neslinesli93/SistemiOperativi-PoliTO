#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
  int i;

  i = 0;
  while (i<=2 && fork()) {
    if (!fork()) {
      fprintf (stdout, "Running Exec ..."); fflush (stdout);
      execlp ("echo", "echo", "i*i", (char *) 0);
    }
    i++;
    fprintf (stdout, "Running System ..."); fflush (stdout);
    system ("echo i+i");
  }

  return (0);
} 
