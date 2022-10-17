CC = gcc
CFLAGS = -Wall -Wextra -Werror -g 
OBJS = main.o
VPATH = src:

all: dependencies main

main: $(OBJS)

dependencies:
	gcc -MM src/*.c > deps

-include deps


clean:
	rm *.o main deps

.PHONY: clean dependencies
