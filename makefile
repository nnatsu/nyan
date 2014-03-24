CC = gcc
CFLAGS = -c -g -Wall

main: main.o parse.o job.o exec.o
	$(CC) -o main main.o parse.o job.o exec.o -lpthread

clean:
	$(RM) main *.o
