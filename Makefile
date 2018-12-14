output: ipd.o
	gcc my_socket.o arp_linked_list.o ip_linked_list.o misc.o arp_protocol.o ip_protocol.o ipd.o -pthread -o ipd

ipd.o: global_defines.h ipd.h my_socket.o arp_linked_list.o ip_linked_list.o misc.o arp_protocol.o ip_protocol.o ipd.c 
	gcc -c -Wall -pedantic ipd.c

ip_protocol.o: ip_protocol.c ip_protocol.h
	gcc -c -Wall -pedantic ip_protocol.c

my_socket.o: my_socket.c my_socket.h
	gcc -c -Wall -pedantic my_socket.c

arp_protocol.o: arp_protocol.c arp_protocol.h
	gcc -c -Wall -pedantic arp_protocol.c

ip_linked_list.o: ip_linked_list.c ip_linked_list.h
	gcc -c -Wall -pedantic ip_linked_list.c

arp_linked_list.o: arp_linked_list.c arp_linked_list.h
	gcc -c -Wall -pedantic arp_linked_list.c

misc.o: misc.c misc.h
	gcc -c -Wall -pedantic misc.c

clean:
	-rm -f ipd
