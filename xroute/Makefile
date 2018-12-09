output: xroute.o
	gcc xroute.o my_socket.o misc.o -o xroute

xroute.o: ../global_defines.h xroute.h my_socket.o misc.o xroute.c
	gcc -c -Wall -pedantic xroute.c

my_socket.o: ../my_socket.c ../my_socket.h
	gcc -c -Wall -pedantic ../my_socket.c

misc.o: ../misc.c ../misc.h
	gcc -c -Wall -pedantic ../misc.c

clean:
	-rm -f xroute
