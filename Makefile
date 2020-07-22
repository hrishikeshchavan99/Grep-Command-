mygrep: main.o list.o queue.o
	cc main.o queue.o list.o -o mygrep
main.o: main.c list.h queue.h
	cc -Wall -c main.c
list.o: list.c list.h
	cc -Wall -c list.c
queue.o: queue.c queue.h
	cc -Wall -c queue.c
