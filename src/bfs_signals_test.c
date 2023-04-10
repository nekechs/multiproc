#include <stdio.h>
#include "dataset_generator.h"
#include "arraylist.h"
#include "bfs_solver_signals.h"

int main(int argc, char const *argv[])
{
    // bfs
    double bfs_mean;
    int bfs_max;
    int bfs_keys;
    int H = 20;

    FILE *bfs_outfile = fopen("./bfs_signals_output.txt", "w");
    solve_bfs_signals("./xd.txt", &bfs_max, &bfs_mean, &bfs_keys, H, 11, bfs_outfile);
    return 0;
}
