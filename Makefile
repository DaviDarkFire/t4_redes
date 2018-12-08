CC = gcc
# CFLAGS = -Wall -pedantic -std=c99
THREADFLAGS = -pthread
MSC = misc.c
SRCCODES = arp_linked_list.c ip_linked_list.c arp_protocol.c my_socket.c ipd.c

ipd: arp_linked_list.c ip_linked_list.c $(MSC) arp_protocol.c my_socket.c ipd.c
	$(CC)  $(THREADFLAGS) $(MSC) $(SRCCODES) -o $@

clean:
	-rm -f ipd


########

# output: ipd.o
# 	gcc ipd.o my_socket.o arp_linked_list.o misc.o -o ipd
#
# ipd.o: global_defines.h ipd.h my_socket.o arp_linked_list.o misc.o ipd.c
# 	gcc -c -Wall -pedantic ipd.c
#
# my_socket.o: my_socket.c my_socket.h
# 	gcc -c -Wall -pedantic my_socket.c
#
# arp_linked_list.o: arp_linked_list.c arp_linked_list.h
# 	gcc -c -Wall -pedantic arp_linked_list.c
#
# misc.o: misc.c misc.h arp_linked_list.h
# 	gcc -c -Wall -pedantic misc.c
#
# clean:
# 	-rm -f ipd
