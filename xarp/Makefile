CC = gcc
CFLAGS = -Wall -pedantic -g
MSC = ../misc.c

xarp: $(MSC) ../global_defines.h xarp.c
	$(CC) ../global_defines.h $(CFLAGS) $? -o $@

clean:
	-rm -f xarp
