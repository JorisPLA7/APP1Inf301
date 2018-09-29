all: prog_principal

prog_principal: prog_principal.o client.o
	clang -o prog_principal prog_principal.o client.o

prog_principal.o: prog_principal.c client.h
	clang -Wall -Werror -g -c prog_principal.c
	
cryptSeq.o : cryptSeq.c cryptSeq.h
	clang -Wall -Werror -g -c cryptSeq.c

client.o: client.c client.h
	clang -Wall -Werror -g -c client.c

clean:
	rm *.o prog_principal

