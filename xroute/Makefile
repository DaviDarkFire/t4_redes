CC = gcc
CFLAGS = -Wall -pedantic -g
MSC = ../misc.c

xroute: $(MSC) xroute.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	-rm -f xroute
