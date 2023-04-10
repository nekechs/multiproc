#include <stdio.h>
#include <stdlib.h>

#include "dfs_solver_signal.h"

int main(int argc, char ** argv) {
    generate_file("./xd.txt", 50, 10000, 400);
    
    // dfs
    double dfs_mean;
    int dfs_max;

    FILE * dfs_outfile = fopen("./dfs_output.txt", "w");
    solve_dfs_signal("./xd.txt", &dfs_max, &dfs_mean, 30, dfs_outfile);

    return EXIT_SUCCESS;
}