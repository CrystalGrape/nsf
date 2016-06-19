LD=-lpthread -lcjson -lm
DEBUG =
all:nsf.o nsf_config.o nsf_signal.o nsf_epoll.o nsf_event.o nsf_master.o nsf_worker.o module tool
	gcc $(DEBUG) -o nsf *.o modules/*.o ${LD}
nsf.o:
	gcc $(DEBUG) -c nsf.c
nsf_signal.o:
	gcc $(DEBUG) -c nsf_signal.c
nsf_epoll.o:
	gcc $(DEBUG) -c nsf_epoll.c
nsf_config.o:
	gcc $(DEBUG) -c nsf_config.c
nsf_event.o:
	gcc $(DEBUG) -c nsf_event.c
nsf_master.o:
	gcc $(DEBUG) -c nsf_master.c
nsf_worker.o:
	gcc $(DEBUG) -c nsf_worker.c
module:
	make -C modules/
tool:
	make -C tools/
clean:
	-rm *.o
	-rm nsf
	make -C modules/ clean
	make -C tools/ clean
	
