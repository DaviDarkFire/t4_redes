output: xifconfig.o
	gcc xifconfig.o my_socket.o misc.o -o xifconfig

xifconfig.o: ../global_defines.h xifconfig.h my_socket.o misc.o xifconfig.c
	gcc -c -Wall -pedantic xifconfig.c

my_socket.o: ../my_socket.c ../my_socket.h
	gcc -c -Wall -pedantic ../my_socket.c

misc.o: ../misc.c ../misc.h
	gcc -c -Wall -pedantic ../misc.c

clean:
	-rm -f xifconfig
