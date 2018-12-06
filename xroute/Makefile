CC = gcc
CFLAGS = -Wall -pedantic -g
MSC = ../misc.c
SCKT = ../my_socket.c
xroute: $(MSC) $(SCKT) xroute.c
	$(CC) $(SCKT) $(CFLAGS) $? -o $@

clean:
	-rm -f xroute
