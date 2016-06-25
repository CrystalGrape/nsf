all:master talkserver
.PHONY:master talkserver
master:
	make -C master/
.PHONY:talkserver
talkserver:
	make -C talkserver/
.PHONY:clean
clean:
	make -C master/ clean
	make -C talkserver/ clean
	-rm *.so
