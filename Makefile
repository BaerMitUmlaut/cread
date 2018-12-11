CC      = gcc
CFLAGS  = -Wall -std=c11 -O3 -pedantic -pthread
CLIBS   = -lm
BIN     = bin/cread

.SECONDEXPANSION:
.PHONY: all clean test
.SILENT:


all: $(BIN)

$(BIN): $$(subst .c,.o,$$(wildcard src/*.[ch]))
	echo "Linking $(@F)"
	mkdir -p bin
	$(CC) $(CFLAGS) $(CLIBS) -o $@ $^

%.o: %.c
	echo "Compiling $^"
	$(CC) $(CFLAGS) $(CLIBS) -g -c $^ -o $@

clean:
	rm -f bin/*
	rm -f src/*.o
