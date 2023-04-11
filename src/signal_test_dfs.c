#include <stdio.h>
#include <stdlib.h>

#include "dfs_solver_signal.h"

int main(int argc, char ** argv) {
    generate_file("./xd.txt", 50, 100000, 400);
    
    // dfs
    double dfs_mean;
    int dfs_max;

    FILE * dfs_outfile = fopen("./dfs_output.txt", "w");
    printf("start\n");
    solve_dfs_signal("./xd.txt", &dfs_max, &dfs_mean, 3, 10,dfs_outfile);
    printf("end\n");

    return EXIT_SUCCESS;
}