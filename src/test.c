#include <stdio.h>
#include "dataset_generator.h"
#include "arraylist.h"
#include "dfs_solver.h"
#include "bfs_solver.h"

int main() {
    // generate data
    generate_file("./xd.txt", 2, 10, 400);
    
    // dfs
    double dfs_mean;
    int dfs_max;

    FILE * dfs_outfile = fopen("./dfs_output.txt", "w");
    solve_dfs("./xd.txt", &dfs_max, &dfs_mean, 11, dfs_outfile);

    // bfs
    double bfs_mean;
    int bfs_max;

    FILE * bfs_outfile = fopen("./bfs_output.txt", "w");
    solve_dfs("./xd.txt", &bfs_max, &bfs_mean, 11, bfs_outfile);

    // printf("%d, %lf\n", dfs_max, dfs_mean);

    // arraylist_t A;
    // al_alloc(&A, sizeof(int), 1);

    // int x = 8;
    // al_insert(&A, A.nmemb, &x);
    // al_insert(&A, A.nmemb, &x);
    // al_insert(&A, A.nmemb, &x);

    // int * arr = A.data;
    // for(int i = 0; i < 3; i++) {
    //     printf("%d\n", arr[i]);
    // }

    // al_free(&A);
}