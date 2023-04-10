#include <stdio.h>
#include "dataset_generator.h"
#include "arraylist.h"
#include "dfs_solver.h"
#include "bfs_solver.h"

int main() {
    // generate data
    generate_file("./xd.txt", 50, 10000, 400);
    
    // dfs
    double dfs_mean;
    int dfs_max;

    FILE * dfs_outfile = fopen("./dfs_output.txt", "w");
    solve_dfs("./xd.txt", &dfs_max, &dfs_mean, 30, dfs_outfile);

    // bfs
    double bfs_mean;
    int bfs_max;

    FILE * bfs_outfile = fopen("./bfs_output.txt", "w");
    solve_bfs("./xd.txt", &bfs_max, &bfs_mean, 11, bfs_outfile);
}