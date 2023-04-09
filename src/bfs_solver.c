#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "bfs_solver.h"
#include "arraylist.h"

// int nums_length;
// int* nums;

void fork_tree(int n, int id, int* max_pipe, int* avg_pipe, int* nums, int nums_length, FILE* outfile) {

    // printf("Process with ID %d created\n", id);
    fprintf(outfile, "Hi, I am process %d with return arg %d, and my parent is %d.\n", getpid(), id, getppid());

    // fork left child
    int max_pipe_l[2];
    int avg_pipe_l[2];
    if (id * 2 + 1 < n){
        pipe(max_pipe_l);
        pipe(avg_pipe_l);
        pid_t left_child = fork();
        if (left_child == 0) {
            fork_tree(n, id * 2 + 1, max_pipe_l, avg_pipe_l, nums, nums_length, outfile);
            _exit(0);
        }
    }

    // fork right child
    int max_pipe_r[2];
    int avg_pipe_r[2];
    if (id * 2 + 2 < n){
        pipe(max_pipe_r);
        pipe(avg_pipe_r);
        pid_t right_child = fork();
        if (right_child == 0) {
            fork_tree(n, id * 2 + 2, max_pipe_r, avg_pipe_r, nums, nums_length, outfile);
            _exit(0);
        }
    }

    // get max, avg, and keys!
    int s_i = (nums_length/n)*id; 
    int e_i = (nums_length/n)*(id+1);

    int max = INT_MIN;
    int sum = 0;
    int num_keys = 0;
    for (int i = s_i; i < e_i; i++)
    {    
        if (nums[i] == -1){
            num_keys += 1;
            fprintf(outfile, "Hi, I am process %d with return arg %d. I found the hidden key in position A[%d]\n", getpid(), n, i);
        }

        if (nums[i] > max)
        {
            max = nums[i];
        }
        sum += nums[i];
    }
    double avg = ((double)sum ) / ((double)nums_length);

    // printf("id:%d [%d-%d) max:%d avg:%f\n", id, s_i, e_i, max, avg);

    // wait for left child
    if (id * 2 + 1 < n){
        int max_l;
        double avg_l;

        read(max_pipe_l[0], &max_l, sizeof(max_l));
        read(avg_pipe_l[0], &avg_l, sizeof(avg_l));

        if (max_l > max)
        {
            max = max_l;
        }
        
        avg += avg_l;

        wait(NULL);
    }

    // wait for right child
    if (id * 2 + 2 < n){
        int max_r;
        double avg_r;

        read(max_pipe_r[0], &max_r, sizeof(max_r));
        read(avg_pipe_r[0], &avg_r, sizeof(avg_r));

        if (max_r > max)
        {
            max = max_r;
        }
        
        avg += avg_r;

        wait(NULL);
    }

    // write result to pipes
    write(max_pipe[1], &max, sizeof(max));
    write(avg_pipe[1], &avg, sizeof(avg));

    // exit if not main parent
    if (id != 0)
    {
        _exit(id);
    }
}

int solve_bfs(char * filename, int * max, double * avg, int pn, FILE * outfile){

    // load data from file into array called nums
    arraylist_t data;
    al_alloc(&data, sizeof(int), 1);
    FILE* file = fopen(filename, "r");
    int num;
    int nums_length = 0;
    while (fscanf(file, "%d\n", &num) > 0){
        al_insert(&data, data.nmemb, &num);
        nums_length += 1;
    }
    fclose(file);
    int* nums = (int*)data.data;

    // pipes
    int avg_pipe[2];
    int max_pipe[2];
    pipe(max_pipe);
    pipe(avg_pipe);

    // bfs tree 
    fork_tree(pn, 0, max_pipe, avg_pipe, nums, nums_length, outfile);

    // get result
    read(max_pipe[0], &max, sizeof(*max));
    read(avg_pipe[0], &avg, sizeof(*avg));

    // output result
    fprintf(outfile, "Max=%d, Avg=%lf\n", *max, *avg);
    // printf("max: %d\n", max);
    // printf("avg: %f\n", avg);
    return 0;
}

// int main() {

//     char* filename = "./xd.txt";

//     // load data into array called nums
//     arraylist_t data;
//     al_alloc(&data, sizeof(int), 1);
//     FILE* file = fopen(filename, "r");
//     int num;
//     nums_length = 0;
//     while (fscanf(file, "%d\n", &num) > 0){
//         al_insert(&data, data.nmemb, &num);
//         nums_length += 1;
//     }
//     fclose(file);
//     nums = (int*)data.data;

//     // tree size
//     int n = 5;

//     // pipes
//     int avg_pipe[2];
//     int max_pipe[2];
//     pipe(max_pipe);
//     pipe(avg_pipe);

//     // bfs tree 
//     fork_tree(n, 0, max_pipe, avg_pipe);

//     // get result
//     int max;
//     double avg;
//     read(max_pipe[0], &max, sizeof(max));
//     read(avg_pipe[0], &avg, sizeof(avg));

//     printf("max: %d\n", max);
//     printf("avg: %f\n", avg);

//     return 0;
// }