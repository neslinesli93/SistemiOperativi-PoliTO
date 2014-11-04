#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
  char str[100];
  int i;
 
  fork();
  for (i=0; i<2; i++){
	  if (fork()!=0) {
		  sprintf (str, "exec with i=%d", i);
		  execlp ("echo", "myPgrm", str, NULL);
	  } else {
		  sprintf (str, "echo system with i=%d", i);
		  system (str);
	  }
  }
  return (0);
}
