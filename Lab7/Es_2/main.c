#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

typedef struct
{
	char *fname_in;
	char *fname_out;
} thread_params_t;

void *thread_rw(void *param)
{
	thread_params_t *p;
	FILE *fin, *fout;
	int n, *v, i, j, temp;
	void *retVal;

	p = (thread_params_t *) param;
	fin = fopen(p->fname_in, "r");
	fout = fopen(p->fname_out, "w");

	if (fin == NULL || fout == NULL)
	{
		fprintf(stderr, "Error while opening/closing the files\n");
		pthread_exit(retVal);
	}
	
	fscanf(fin, "%d", &n);
	v = malloc(sizeof(int) * n);

	for (i = 0; i < n; i++)
		fscanf(fin, "%d", &v[i]);
	fclose(fin);
 
	// Bubble fucking sort
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n - i - 1; j++)
		{
			if (v[j] > v[j+1])
			{
				temp = v[j];
				v[j] = v[j+1];
				v[j+1] = temp;
			}
		}
	}

	for (i = 0; i < n; i++)
		fprintf(fout, "%d\n", v[i]);
	fclose(fout);

	free(v);
	pthread_exit(retVal);
}

int main(int argc, char *argv[])
{
	int n, i, len;
	thread_params_t *v;
	char *fin, *fout;
	pthread_t *tids;
	void *retVal;
	
	if (argc < 4)
	{
		fprintf(stdout, "Usage: %s <n> <f_in> <f_out>\n", argv[0]);
		exit(1);
	}

	len = strlen(argv[1]) + 1;
	n = atoi(argv[1]);

	v  = malloc(sizeof(thread_params_t) * n);
	tids = malloc(sizeof(pthread_t) * n);

	for(i = 0; i < n; i++)
	{
		v[i].fname_in = malloc(sizeof(char) * (strlen(argv[2] + len + 4 + 1)));
		sprintf(v[i].fname_in, "%s%d.txt", argv[2], i);

		v[i].fname_out = malloc(sizeof(char) * (strlen(argv[3] + len + 4 + 1)));
		sprintf(v[i].fname_out, "%s%d.txt", argv[3], i);

		pthread_create(&tids[i], NULL, thread_rw, (void *) &v[i]);
	}

	for (i = 0; i < n; i++)
		pthread_join(tids[i], &retVal);

	for(i = 0; i < n; i++)
	{
		free(v[i].fname_in);
		free(v[i].fname_out);
	}
	free(v);
	free(tids);

	return 0;
}
