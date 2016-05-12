INC=-I../
all:module_init.o talksrv
module_init.o:
	gcc -c module_init.c ${INC}
talksrv:
	cd talkserver&&make
clean:
	-rm *.o
