CC = gcc
CFLAGS = -g -Wall

All:	mysh

mysh:	main.o parse.o job.o exec.o
	$(CC) main.o parse.o job.o exec.o -o mysh

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

parse.o:parse.c parse.h
	$(CC) $(CFLAGS) -c parse.c

job.o: job.c
	$(CC) $(CFLAGS) -c job.c

exec.o: exec.c
	$(CC) $(CFLAGS) -c exec.c

clean:
	rm *.o mysh