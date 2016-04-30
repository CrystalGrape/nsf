LD=-lpthread
all:nsf.o nsf_config.o nsf_signal.o nsf_epoll.o nsf_event.o module
	gcc -g -o nsf *.o modules/*.o ${LD}
nsf.o:
	gcc -c nsf.c
nsf_signal.o:
	gcc -c nsf_signal.c
nsf_epoll.o:
	gcc -c nsf_epoll.c
nsf_config.o:
	gcc -c nsf_config.c
nsf_event.o:
	gcc -c nsf_event.c
module:
	make -C modules/
clean:
	-rm *.o
	-rm nsf
	make -C modules/ clean
	
