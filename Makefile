SRCDIR := ./src

CC = gcc
CFLAGS = -std=c99 -Wall -g -fsanitize=address,undefined -Iinclude -DDEBUG=0

VPATH = src:../include include

all: test

test: dataset_generator.o random.o test.o arraylist.o dfs_solver.o
	$(CC) $(CFLAGS) -lm $^ -o $@

test1.c:
	$(CC) $(CFLAGS) $^ -o $@

test.o: dataset_generator.h arraylist.h dfs_solver.h
test1.o: random.h
dfs_solver.o: dfs_solver.h arraylist.h
dataset_generator.o: random.h dataset_generator.h
random.o: random.h
arraylist.o: arraylist.h

clean:
	rm -rf *.o
	rm test