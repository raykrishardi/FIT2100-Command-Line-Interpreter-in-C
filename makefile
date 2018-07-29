OBJS = main.o shell.o
CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall

main: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o main

main.o: main.c shell.h
	$(CC) $(CFLAGS) main.c

shell.o: shell.c shell.h
	$(CC) $(CFLAGS) shell.c

clean:
	touch *