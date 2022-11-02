CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
OBJS = main.o error.o error_infrastructure.o htab.o vector.o scaner.o syntax_tree.o syntactic_analysis.o interner.o
SOURCES = src/*.c src/htab_lib/*.c
HEADERS = src/headers/*.h
LINTER_CHECKS = clang-analyzer-*,-clang-analyzer-cplusplus*,bugprone-*,
LINTER_CHECKS_PEDANTIC = readability-*,misc-*,modernize-*
VPATH = src:

all: dependencies main

main: $(OBJS)

dependencies:
	$(CC) -MM $(SOURCES) > deps

lint:
	clang-tidy $(SOURCES) $(HEADERS) -checks=$(LINTER_CHECKS)

lint_pedantic:
	clang-tidy $(SOURCES) $(HEADERS) -checks=$(LINTER_CHECKS),$(LINTER_CHECKS_PEDANTIC) -header-filter=.*

-include deps

clean:
	rm *.o main deps

.PHONY: clean dependencies
