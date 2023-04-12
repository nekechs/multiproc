#include <stdio.h>
#include <stdlib.h>

#include "dataset_generator.h"
#include "arraylist.h"
#include "bfs_solver_signal.h"

int main(int argc, char ** argv) {

    // initial parameters
    int L = 10000;
    int H = 30;
    int PN = 5;

    // generate data
    generate_file("./xd.txt", 50, L, 400);

    // bfs
    double bfs_mean;
    int bfs_max;
    int bfs_keys;

    FILE * bfs_outfile = fopen("./bfs_signal_output.txt", "w");
    printf("start\n");
    solve_bfs_signal("./xd.txt", &bfs_max, &bfs_mean, &bfs_keys, PN, H, bfs_outfile);
    printf("end\n");

}