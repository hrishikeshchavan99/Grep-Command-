#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"list.h"
void init(list *l) {
	l->head = NULL;
}
int length(list *l) {
	int cnt = 0;
	node1 *p;
	p = l->head;
	while(p != NULL) {
		cnt++;
		p = p->next;
	}
	return cnt;
}
void insert(list *l, data a) {
	int len;
	node1 *tmp, *p, *q = NULL;
	if(a.lineno < 0 )
		return;
	tmp = (node1 *)malloc(sizeof(node1));
	len = strlen(a.str);
	tmp->d.str = (char *)malloc(strlen(a.str) + 1);
	strcpy(tmp->d.str, a.str);
	tmp->d.str[len] = '\0';
	tmp->d.lineno = a.lineno;
	tmp->next = NULL;
	p = l->head;
	if (!p){
		l->head = tmp;
		return;
	}
	if (p->d.lineno > a.lineno){
		l->head = tmp;
		tmp->next = p;
		return;
	} 
	while (p && (p->d.lineno < a.lineno)){
		q = p;
		p = p->next;
	}
	if (!p){
		q->next = tmp;
		return;
	}
	if (p->d.lineno == a.lineno) {
		return;
	}
	tmp->next = q->next;
	q->next = tmp;
}
void remov(list *l, int pos) {
	int ls;
	node1 *tmp, *p;
	ls = length(l);
	p = l->head;
	tmp = p;
	if (ls == 0)
		return;
	if (ls == 1){
		if (p->d.lineno == pos){
			l->head = NULL;
			free(p);
			return;
		}
	} 
	if (p->d.lineno == pos) {
		l->head = p->next;
		free(p);
		return;
	}
	while (p) {
		if (p->d.lineno < pos) {
			tmp = p;
			p = p->next;
			
		}
		else
			break;
	}
	if (!p)
		return;
	if (p->d.lineno > pos)
		return;
	if (p->d.lineno == pos) {
		//p is node to be deleted
		if (p->next == NULL)
			tmp->next = NULL;
		else
			tmp->next = p->next;
		free(p);
		return;	

	}
}
void print(list l) {
	node1 *p;
	p = l.head;
	while(p != NULL) {
		printf("%s\n", p->d.str);
		p = p->next;	
	}
}
