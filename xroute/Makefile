# CC = gcc
# CFLAGS = -Wall -pedantic -g
# MSC = ../misc.c
# SCKT = ../my_socket.c
# xroute: $(MSC) $(SCKT) xroute.c
# 	$(CC) $(SCKT) $(CFLAGS) $? -o $@

output: xroute.o
	gcc xroute.o my_socket.o misc.o -o xroute

xroute.o: ../global_defines.h xroute.h my_socket.o misc.o
	gcc -c -Wall -pedantic xroute.c

my_socket.o: ../my_socket.c ../my_socket.h
	gcc -c -Wall -pedantic ../my_socket.c

misc.o: ../misc.c ../misc.h ../arp_linked_list.h
	gcc -c -Wall -pedantic ../misc.c

clean:
	-rm -f xroute
