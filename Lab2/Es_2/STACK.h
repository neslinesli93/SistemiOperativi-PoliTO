typedef char * Item;
typedef struct STACK *ST;

ST STinit();
int STempty(ST st);
void STpush(ST st, Item i);
Item STpop(ST st);

void STprint(ST st);
