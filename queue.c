#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"
void qinit(queue *q) {
	q->head = q->tail = NULL;
}
void enq(queue *q, char *s){
	node *p = (node *)malloc(sizeof(node));
	strcpy(p->name, s);
	if (q->head == NULL) {
		q->head = q->tail = p;
		p->next = p;
		return;
	}
	q->tail->next = p;
	q->tail = p;
	p->next = q->head;	
}
char *deq(queue *q) {
	char *s = (char *)malloc(64 * sizeof(char));
	node *p;
	p = q->head;
	strcpy(s, p->name);
	if (q->head == q->tail) {
		q->head = q->tail = NULL;
		free(p);
		return s;
	}
	q->head = p->next;
	q->tail->next = q->head;
	free(p);
	return s;
}
int qempty(queue *q) {
	return q->head == NULL;
}
int qfull(queue *q) {
	return 0;
}
