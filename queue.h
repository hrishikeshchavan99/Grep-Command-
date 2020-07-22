typedef struct node{
	char name[256];
	struct node *next;
}node;
typedef struct queue{
	node *head;
	node *tail;
}queue;
void qinit(queue *q);
void enq(queue *q, char *s);
char *deq(queue *q);
int qfull(queue *q);
int qempty(queue *q);
