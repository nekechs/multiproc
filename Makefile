SRCDIR := ./src

CC = gcc
CFLAGS = -std=c99 -Wall -g -fsanitize=address,undefined -Iinclude -DDEBUG=0

VPATH = src:../include include

all: test signal_test

# link all of the object files
test: dataset_generator.o random.o test.o arraylist.o dfs_solver.o bfs_solver.o
	$(CC) $(CFLAGS) -lm $^ -o $@

signal_test: dataset_generator.o random.o signal_test.o arraylist.o dfs_solver_signal.o
	$(CC) $(CFLAGS) -lm $^ -o $@

#---------------------------------------------------
# compile each source file into the object file
test.o: dataset_generator.h arraylist.h dfs_solver.h
signal_test.o: dataset_generator.h arraylist.h dfs_solver_signal.h

dfs_solver.o: dfs_solver.h arraylist.h
bfs_solver.o: bfs_solver.h arraylist.h
dataset_generator.o: random.h dataset_generator.h
random.o: random.h
arraylist.o: arraylist.h

dfs_solver_signal.o: dfs_solver_signal.h dfs_solver.h arraylist.h 

clean:
	rm -rf *.o