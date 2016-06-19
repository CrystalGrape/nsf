INC=-I../
all:module_init.o talksrv
module_init.o:
	gcc -c module_init.c ${INC}
talksrv:
	make -C talkserver
nsql:
	make -C sql
clean:
	-rm *.o
