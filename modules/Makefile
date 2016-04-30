INC=-I../
all:module_init.o reply.o
reply.o:
	gcc -c reply.c ${INC}
module_init.o:
	gcc -c module_init.c ${INC}
clean:
	-rm *.o
