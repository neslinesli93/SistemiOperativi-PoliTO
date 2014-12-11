#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#define MAX_ELEM 512

typedef struct
{
	int *v;
	int len;
} thread_retval_t;

void bubblesort(int *v, int len)
{
	int i, j, temp;
	
	for (i = 0; i < len; i++)
	{
		for (j = 0; j < len - i - 1; j++)
		{
			if (v[j] > v[j+1])
			{
				temp = v[j];
				v[j] = v[j+1];
				v[j+1] = temp;
			}
		}
	}
}

void *thread_rw(void *fname)
{
	FILE *fp;
	int n, *v, i, j, temp;
	thread_retval_t *retVal;

	fp = fopen((char *) fname, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Error while opening/closing the files\n");
		pthread_exit((void *) 0);
	}
	
	fscanf(fp, "%d", &n);
	v = malloc(sizeof(int) * n);

	for (i = 0; i < n; i++)
		fscanf(fp, "%d", &v[i]);
	fclose(fp);
 
	bubblesort(v, n);

	retVal = malloc(sizeof(thread_retval_t));
	retVal->v = v;
	retVal->len = n;

	pthread_exit((void *) retVal);
}


int main(int argc, char *argv[])
{
	int n, i, j, len, *v, lastdone;
	char *fin;
	pthread_t *tids;
	thread_retval_t *retVal;
	FILE *fout;
	
	if (argc < 4)
	{
		fprintf(stdout, "Usage: %s <n> <f_in> <f_out>\n", argv[0]);
		exit(1);
	}

	fout = fopen(argv[3], "w");
	if (fout == NULL)
	{
		fprintf(stderr, "Error while opening the file\n");
		exit(1);
	}

	len = strlen(argv[1]) + 1;
	n = atoi(argv[1]);

	tids = malloc(sizeof(pthread_t) * n);
	retVal = malloc(sizeof(thread_retval_t));
	v = malloc(sizeof(int) * MAX_ELEM);

	for (i = 0; i < n; i++)
	{
		fin = malloc(sizeof(char) * (strlen(argv[2] + len + 4 + 1)));
		sprintf(fin, "%s%d.txt", argv[2], i);
		pthread_create(&tids[i], NULL, thread_rw, (void *) fin);
	}

	lastdone = 0;
	for (i = 0; i < n; i++)
	{
		pthread_join(tids[i], (void **) &retVal);
		for (j = 0; j < retVal->len; j++)
			v[j+lastdone] = retVal->v[j];

		lastdone += retVal->len;
		free(retVal->v);
	}

	bubblesort(v, lastdone);
	for (i = 0; i < lastdone; i++)
		fprintf(fout, "%d\n", v[i]);
	fclose(fout);

	free(tids);
	free(v);
	free(retVal);

	return 0;
}
