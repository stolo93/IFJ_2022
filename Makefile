CC = gcc
CFLAGS = -Wall -Wextra -Werror -g --warn-no-unused-function
OBJS = main.o error.o error_infrastructure.o symtable.o vector.o scaner.o syntax_tree.o interner.o syntactic_analysis.o semantic_analysis.o semantic_expression.o
SOURCES = src/*.c
HEADERS = src/headers/*.h
LINTER_CHECKS = clang-analyzer-*,-clang-analyzer-cplusplus*,bugprone-*,-misc-no-recursion,-bugprone-reserved-identifier,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling
LINTER_CHECKS_PEDANTIC = readability-*,misc-*,modernize-*,-misc-no-recursion,-bugprone-reserved-identifier,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling
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

.PHONY: clean dependencies lint lint_pedantic
