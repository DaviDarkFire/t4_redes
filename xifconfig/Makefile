CC = gcc
CFLAGS = -Wall -pedantic -g
MSC = ../misc.c

xifconfig: $(MSC) xifconfig.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	-rm -f xifconfig
