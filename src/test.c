#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "dataset_generator.h"
#include "arraylist.h"
#include "dfs_solver.h"
#include "bfs_solver.h"

#define TIMEVAL_DIFF(tv_init, tv_final) ((double)((tv_final).tv_sec) + (double)((tv_final).tv_usec) / 1000000) - ((double)((tv_init).tv_sec) + (double)((tv_init).tv_usec) / 1000000)
#define INPUT_FILE "./xd.txt"

int main(int argc, char ** argv) {
    /* First, we need to gather the arguments. */
    if(argc != 4) {
        printf("Error: Must supply 3 arguments: L, H, and PN\n");
        return EXIT_FAILURE;
    }

    int L = atoi(argv[1]);
    int H = atoi(argv[2]);
    int PN = atoi(argv[3]);

    struct timeval tv_init;
    struct timeval tv_final;
   
    // generate data
    generate_file(INPUT_FILE, 50, L, 400);
    
    /* ------------------------------------------------------------- */
    // dfs
    double dfs_mean;
    int dfs_max;

    FILE * dfs_outfile = fopen("./dfs_output.txt", "w");

    gettimeofday(&tv_init, NULL);
    solve_dfs(INPUT_FILE, &dfs_max, &dfs_mean, PN, H, dfs_outfile);
    gettimeofday(&tv_final, NULL);
    printf("DFS took time %lf\n", TIMEVAL_DIFF(tv_init, tv_final));

    /* ------------------------------------------------------------- */
    // bfs
    double bfs_mean;
    int bfs_max;

    FILE * bfs_outfile = fopen("./bfs_output.txt", "w");

    gettimeofday(&tv_init, NULL);
    solve_bfs(INPUT_FILE, &bfs_max, &bfs_mean, PN, H, bfs_outfile);
    gettimeofday(&tv_final, NULL);
    printf("BFS took time %lf\n", TIMEVAL_DIFF(tv_init, tv_final));

}