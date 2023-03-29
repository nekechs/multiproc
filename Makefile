SRCDIR := ./src

CC = gcc
CFLAGS = -std=c99 -Wall -g -fsanitize=address,undefined -Iinclude -DDEBUG=0

VPATH = src:../include include

# CPPFLAGS = -DTEST

# debug:
# 	CPPFLAGS += -DDEBUG=1

all: test

test: dataset_generator.o random.o test.o
	$(CC) $(CFLAGS) -lm $^ -o $@

test.o: dataset_generator.h
dataset_generator.o: random.h dataset_generator.h
random.o: random.h

clean:
	rm -rf *.o
	rm test