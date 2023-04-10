#ifndef _DFS_SOLVER_H_
#define _DFS_SOLVER_H_

struct dfs_chunk {
    int nmemb;
    int max;
    double mean;
    int num_key;
};

struct dfs_chunk dfs_chunk_merge(const struct dfs_chunk * left, const struct dfs_chunk * right);

int solve_dfs(char * filename, int * max, double * avg, int pn, int H, FILE * outfile);

#endif