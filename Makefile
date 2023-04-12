SRCDIR := ./src

CC = gcc
CFLAGS = -std=gnu99 -Wall -g -fsanitize=address,undefined -Iinclude -DDEBUG=0

VPATH = src:../include include

all: test.out signal_test_dfs.out signal_test_bfs.out memtest.out

# link all of the object files
test.out: dataset_generator.o random.o test.o arraylist.o dfs_solver.o bfs_solver.o memshare.o
	$(CC) $(CFLAGS) -lm -lpthread $^ -o $@

signal_test_dfs.out: signal_test_dfs.o dataset_generator.o random.o arraylist.o dfs_solver_signal.o memshare.o
	$(CC) $(CFLAGS) -lm -lpthread $^ -o $@

signal_test_bfs.out: signal_test_bfs.o dataset_generator.o random.o arraylist.o bfs_solver_signal.o memshare.o
	$(CC) $(CFLAGS) -lm -lpthread $^ -o $@

memtest.out: memtest.o memshare.o
	$(CC) $(CFLAGS) -lpthread $^ -o $@
#---------------------------------------------------
# compile each source file into the object file
test.o: dataset_generator.h arraylist.h dfs_solver.h bfs_solver.h
signal_test_dfs.o: dataset_generator.h arraylist.h dfs_solver_signal.h
signal_test_bfs.o: dataset_generator.h arraylist.h bfs_solver_signal.h

dfs_solver.o: dfs_solver.h arraylist.h memshare.h
bfs_solver.o: bfs_solver.h arraylist.h memshare.h
dfs_solver_signal.o: dfs_solver_signal.h dfs_solver.h arraylist.h 
bfs_solver_signal.o: bfs_solver_signal.h bfs_solver.h arraylist.h

memtest.o: memshare.h

dataset_generator.o: random.h dataset_generator.h
random.o: random.h
arraylist.o: arraylist.h
memshare.o: memshare.h

clean:
	rm -rf *.o
	rm -rf *.txt
	rm -rf *.out