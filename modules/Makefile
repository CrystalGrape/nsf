all:master talkserver http_module
.PHONY:master
master:
	make -C master/
.PHONY:talkserver
talkserver:
	make -C talkserver/
.PHONY:http_module
http_module:
	make -C http_module/
.PHONY:clean
clean:
	-rm *.so
	-make -C master/ clean
	-make -C talkserver/ clean
	-make -C http_module/ clean
