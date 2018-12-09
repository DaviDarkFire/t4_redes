output: xarp.o
	gcc xarp.o my_socket.o misc.o -o xarp

xarp.o: ../global_defines.h xarp.h my_socket.o misc.o xarp.c
	gcc -c -Wall -pedantic xarp.c

my_socket.o: ../my_socket.c ../my_socket.h
	gcc -c -Wall -pedantic ../my_socket.c

misc.o: ../misc.c ../misc.h
	gcc -c -Wall -pedantic ../misc.c

clean:
	-rm -f xarp
