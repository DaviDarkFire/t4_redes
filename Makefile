CC = gcc
# CFLAGS = -Wall -pedantic -std=c99
THREADFLAGS = -pthread
MSC = misc.c
SRCCODES = arp_linked_list.c ip_linked_list.c arp_protocol.c my_socket.c ipd.c

ipd: arp_linked_list.c ip_linked_list.c $(MSC) arp_protocol.c my_socket.c ipd.c
	$(CC)  $(THREADFLAGS) $(MSC) $(SRCCODES) -o $@

clean:
	-rm -f ipd
