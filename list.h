typedef struct data {
	char *str;
	int lineno;
}data;
typedef struct node1 {
	data d;
	struct node1 *next;
}node1;
typedef struct list {
	struct node1 *head;
}list;
void init(list *l);
int length(list *l);
void insert(list *l, data a);
void remov(list *l, int pos);
void print(list l);
