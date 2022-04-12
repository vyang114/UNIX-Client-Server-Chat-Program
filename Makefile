CC = gcc
CFLAGS = -g -Wall

all: s-talk

s-talk: main.o list.o sender.o receiver.o terminate.o
	$(CC) $(CFLAGS) -pthread -o s-talk main.o list.o sender.o receiver.o terminate.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

list.o: list.c
	$(CC) $(CFLAGS) -c list.c

sender.o: sender.c
	$(CC) $(CFLAGS) -c sender.c

receiver.o: receiver.c
	$(CC) $(CFLAGS) -c receiver.c

terminate.o: terminate.c
	$(CC) $(CFLAGS) -c terminate.c

clean:
	-rm s-talk main.o list.o sender.o receiver.o terminate.o
