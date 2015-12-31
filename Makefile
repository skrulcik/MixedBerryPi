#
# MixedBerryPi Makefile
#
CC = gcc
CFLAGS = -g -Wall -DDEBUG=1
LDFLAGS = -lpthread -lwiringPi

all: lightsserver

color.o: color.c color.h csapp.h
	$(CC) $(CFLAGS) -c color.c

rgbstrip.o: rgbstrip.c rgbstrip.h color.h
	$(CC) $(CFLAGS) -c rgbstrip.c $(LDFLAGS)

csapp.o: csapp.c csapp.h
	$(CC) $(CFLAGS) -c csapp.c $(LDFLAGS)

lightsserver.o: lightsserver.c csapp.h
	$(CC) $(CFLAGS) -c lightsserver.c $(LDFLAGS)

lightsserver: lightsserver.o csapp.o rgbstrip.o color.o

rgb_client.o: client/rgb_client.c csapp.h
	$(CC) $(CFLAGS) -c client/rgb_client.c

rgb_client: rgb_client.o csapp.o

clean:
	rm -f *~ *.o lightsserver rgb_client

