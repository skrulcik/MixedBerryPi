#
# MixedBerryPi Makefile
#
CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread

all: lightsserver

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c

lightsserver.o: lightsserver.c csapp.h
	$(CC) $(CFLAGS) -c lightsserver.c

lightsserver: lightsserver.o csapp.o

clean:
	rm -f *~ *.o lightsserver

