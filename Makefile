SRCDIR := ./src

CC = gcc
CFLAGS = -std=c99 -Wall -g -fsanitize=address,undefined -Iinclude -DDEBUG=1

VPATH = src:../include include

all: test

test: dataset_generator.o random.o test.o arraylist.o
	$(CC) $(CFLAGS) -lm $^ -o $@

test.o: dataset_generator.h arraylist.h
dataset_generator.o: random.h dataset_generator.h
random.o: random.h
arraylist.o: arraylist.h

clean:
	rm -rf *.o
	rm test