#include <stdio.h>
#include "dataset_generator.h"
#include "arraylist.h"
#include "dfs_solver.h"

int main() {
    generate_file("./xd.txt", 2, 10, 400);
    
    double dfs_mean;
    int dfs_max;

    FILE * ptr = fopen("./dfs_output.txt", "w");
    solve_dfs("./xd.txt", &dfs_max, &dfs_mean, 11, stdout);

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