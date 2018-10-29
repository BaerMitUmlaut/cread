CC      = gcc
CFLAGS  = -Wall -std=c11 -O3 -pedantic
CLIBS   = -lm
BINS    = bin/cread

.SECONDEXPANSION:
.PHONY: all clean test
.SILENT:


all: $(BINS)

bin/cread: $$(subst .c,.o,$$(wildcard src/*.[ch]))
	echo "Linking $(@F)"
	$(CC) $(CFLAGS) $(CLIBS) -o $@ $^

%.o: %.c
	echo "Compiling $^"
	$(CC) $(CFLAGS) $(CLIBS) -g -c $^ -o $@

clean:
	rm -f bin/*
	rm -f src/*.o
