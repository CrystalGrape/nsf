C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
CC = gcc
LD = ld
C_FLAGS = -c -Wall -I../../
LD_FLAGS = -shared -fPIC
GOAL	= libmaster.so

all:$(C_OBJECTS) link
	cp $(GOAL) ../
.c.o:
	$(CC) $(C_FLAGS) $< -o $@
link:
	$(CC) $(LD_FLAGS) $(C_OBJECTS) -o $(GOAL)
.PHONY:clean
clean:
	-rm $(C_OBJECTS)
	-rm $(GOAL)

