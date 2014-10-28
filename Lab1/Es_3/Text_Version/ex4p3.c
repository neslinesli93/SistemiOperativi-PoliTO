#include <stdio.h>
#include <stdlib.h>

#define LEN_LINE 1024

int main (int argc, char* argv[])
{
    FILE *fpin, *fpout;
    char buf[LEN_LINE];

    if (argc < 3)
    {
        printf("Usage: ex4p1.c <file_input> <file_output>\n");
        exit(1);
    }

    fpin = fopen(argv[1], "r");
    if (fpin == NULL)
    {
        printf("diofa in apertura");
        exit(1);
    }

    fpout = fopen(argv[2], "w");

    while(fgets(buf, LEN_LINE, fpin) != NULL)
    {
        fputs(buf, fpout);
    }

    fclose(fpin);
    fclose(fpout);

    return 0;
}
