#ifndef _DFS_SOLVER_SIGNAL_H_
#define _DFS_SOLVER_SIGNAL_H_

#include <sys/types.h>

typedef struct {
    pid_t discoverer;
    int return_arg;
    int index;
} key_md_t;

struct dfs_chunk_signal {
    int nmemb;
    int max;
    double mean;
    int num_key;
    pid_t child_pid;
    int num_hidden_numbers;
};

struct dfs_chunk_signal dfs_chunk_signal_merge(const struct dfs_chunk_signal * left, const struct dfs_chunk_signal * right);

int solve_dfs_signal(char * filename, int * max, double * avg, int pn, int H, FILE * outfile);

#endif