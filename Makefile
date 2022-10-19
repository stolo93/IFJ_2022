CC = gcc
CFLAGS = -Wall -Wextra -Werror -g 
OBJS = main.o error.o error_infrastructure.o htab.o
VPATH = src:

all: dependencies main 

main: $(OBJS)

dependencies:
	$(CC) -MM src/*.c src/htab_lib/*.c > deps

-include deps

clean:
	rm *.o main deps

.PHONY: clean dependencies
