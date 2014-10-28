#include <stdlib.h>
#include <stdio.h>
#include "STACK.h"

typedef struct STnode *link;
struct STnode
{
    Item item;
    link next;
};

struct STACK
{
    link head;
	int n;
};

ST STinit()
{
    ST st = malloc(sizeof *st);
    st->head = NULL;

    return st;
}

int STempty(ST st)
{
    return st->head == NULL;
}

void STpush(ST st, Item i)
{
    link x = malloc(sizeof *x);
    x->item = i; x->next = st->head;
    st->head = x;
}

Item STpop(ST st)
{
    Item i = st->head->item;
    link t = st->head->next;
    free(st->head); st->head = t;

    return i;
}

void STprint(ST st)
{
    Item i;

    if (STempty(st)) return;

    i = STpop(st);
    STprint(st);
    printf("%s\n", i);
}


