#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    int fin, fout;
    int nr, nw;
    char buf[BUFSIZE];

    if (argc != 3)
    {
        printf("Usage: ex4p4 <FILE_IN> <FILE_OUT>\n");
        exit(1);
    }

    fin = open(argv[1], O_RDONLY);
    fout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fin == -1 || fout == -1)
    {
        fprintf(stdout, "Error while opening the files\n");
        exit(1);
    }

    do
    {
        nr = read(fin, buf, BUFSIZE);
        nw = write(fout, buf, nr);
    }
    while (nr == BUFSIZE);

    if (close(fin) || close(fout))
    {
        fprintf(stderr, "Error while closing files");
        exit(1);
    }

    return 0;
}
