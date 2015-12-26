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

rgb_client.o: client/rgb_client.c csapp.h
	$(CC) $(CFLAGS) -c client/rgb_client.c

rgb_client: rgb_client.o csapp.o

clean:
	rm -f *~ *.o lightsserver rgb_client

