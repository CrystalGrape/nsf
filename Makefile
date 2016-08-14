C_SOURCES = $(shell ls *.c)
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
CC = gcc
LD_LIBARY = -lpthread -lcjson -lm -ldl
C_FLAGS = -c -Wall -I../
GOAL	= nsf

all:$(C_OBJECTS) link
.c.o:
	$(CC) $(C_FLAGS) $< -o $@
link:
	$(CC) $(C_OBJECTS) -o $(GOAL) $(LD_LIBARY)
.PHONY:module
module:
	make -C modules/

.PHONY:tools
tools:
	make -C tools/
.PHONY:clean
clean:
	-rm $(C_OBJECTS)
	-rm $(GOAL)
	make -C tools/ clean

