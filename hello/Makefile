all: hello_world

hello_world: hello_world.o client.o
	clang -o hello_world hello_world.o client.o

hello_world.o: hello_world.c client.h
	clang -Wall -Werror -g -c hello_world.c

client.o: client.c client.h
	clang -Wall -Werror -g -c client.c

clean:
	rm *.o hello_world

