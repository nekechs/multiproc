#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>

#include "bfs_solver_signal.h"
#include "arraylist.h"


void handle_SIGCONT(int signum)
{
    // handle SIGCONT the same in all cases for now
    system("pstree");
    sleep(100);
}

void fork_tree(int n, int id, int *max_pipe, int *avg_pipe, int *keys_pipe, int *nums, int nums_length, int H, FILE *outfile)
{

    // printf("Process with ID %d created\n", id);
    fprintf(outfile, "Hi, I am process %d with return arg %d, and my parent is %d.\n", getpid(), id, getppid());
    fflush(outfile);

    // what the child's ids should be (if they will be created)
    int left_child_id  = id * 2 + 1;
    int right_child_id = id * 2 + 2;

    // check if the left/right child should be created (also can be used to see if the child exists)
    bool left_child  = (left_child_id  < n);
    bool right_child = (right_child_id < n);

    // set up signal handler for 
    signal(SIGCONT, handle_SIGCONT);

    // fork left child
    int max_pipe_l[2];
    int avg_pipe_l[2];
    int keys_pipe_l[2];
    pid_t left_child_pid;
    if (left_child)
    {
        pipe(max_pipe_l);
        pipe(avg_pipe_l);
        pipe(keys_pipe_l);
        pid_t left_child = fork();
        left_child_pid = left_child;
        if (left_child == 0)
        {
            fork_tree(n, id * 2 + 1, max_pipe_l, avg_pipe_l, keys_pipe_l, nums, nums_length, H, outfile);
            _exit(0);
        }
    }

    // fork right child
    int max_pipe_r[2];
    int avg_pipe_r[2];
    int keys_pipe_r[2];
    pid_t right_child_pid;
    if (right_child)
    {
        pipe(max_pipe_r);
        pipe(avg_pipe_r);
        pipe(keys_pipe_r);
        pid_t right_child = fork();
        right_child_pid = right_child;
        if (right_child == 0)
        {
            fork_tree(n, id * 2 + 2, max_pipe_r, avg_pipe_r, keys_pipe_r, nums, nums_length, H, outfile);
            _exit(0);
        }
    }

    // get max, avg, and keys!
    int s_i = (id * nums_length) / n;
    int e_i = ((id + 1) * nums_length) / n;

    int max = INT_MIN;
    int sum = 0;
    int keys = 0;
    for (int i = s_i; i < e_i; i++)
    {
        if (nums[i] == -1)
        {
            keys += 1;
            fprintf(outfile, "Hi, I am process %d with return arg %d. I found the hidden key in position A[%d]\n", getpid(), id, i);
            fflush(outfile);
        }

        if (nums[i] > max)
        {
            max = nums[i];
        }
        sum += nums[i];
    }
    double avg = ((double)sum) / ((double)nums_length);

    int max_p = max;

    // wait for left child
    int max_l;
    double avg_l;
    int keys_l;
    if (left_child)
    {
        read(max_pipe_l[0], &max_l, sizeof(max_l));
        read(avg_pipe_l[0], &avg_l, sizeof(avg_l));
        read(keys_pipe_l[0], &keys_l, sizeof(keys_l));
        
        if (max_l > max)
        {
            max = max_l;
        }

        if (keys_l > 0){
            keys += 1;
        }

        avg += avg_l;
    }

    // wait for right child
    int max_r;
    double avg_r;
    int keys_r;
    if (right_child)
    {
        read(max_pipe_r[0], &max_r, sizeof(max_r));
        read(avg_pipe_r[0], &avg_r, sizeof(avg_r));
        read(keys_pipe_r[0], &keys_r, sizeof(keys_r));

        if (max_r > max)
        {
            max = max_r;
        }

        if (keys_r > 0){
            keys += 1;
        }

        avg += avg_r;
    }

    // signals left child
    if (left_child){
        if ((max_l > max_p) || (max_l > max_r)){
            kill(left_child_pid, SIGCONT);
        }
        else if (keys_l >= H){
            kill(left_child_pid, SIGKILL);
        }
        else{
            kill(left_child_pid, SIGCONT);
        }
    }

    // signals right child
    if (right_child){
        if ((max_r > max_p) || (max_r > max_l)){
            kill(right_child_pid, SIGCONT);
        }
        else if (keys_r >= H){
            kill(right_child_pid, SIGKILL);
        }
        else{
            kill(right_child_pid, SIGCONT);
        }
    }

    // write result to pipes
    write(max_pipe[1], &max, sizeof(max));
    write(avg_pipe[1], &avg, sizeof(avg));
    write(keys_pipe[1], &keys, sizeof(keys));

    // exit if not main parent
    if (id != 0)
    {
        // pause child process
        raise(SIGTSTP);
        
        // edit with return of id
        _exit(id);
    }
}

int solve_bfs_signal(char *filename, int *max, double *avg, int *keys, int pn, int H, FILE *outfile)
{
    // load data from file into array called nums
    arraylist_t data;
    al_alloc(&data, sizeof(int), 1);
    FILE *file = fopen(filename, "r");
    int num;
    int nums_length = 0;
    while (fscanf(file, "%d\n", &num) > 0)
    {
        al_insert(&data, data.nmemb, &num);
        nums_length += 1;
    }
    fclose(file);
    int *nums = (int *)data.data;

    // pipes
    int avg_pipe[2];
    int max_pipe[2];
    int keys_pipe[2];
    pipe(max_pipe);
    pipe(avg_pipe);
    pipe(keys_pipe);

    // bfs tree
    fork_tree(pn, 0, max_pipe, avg_pipe, keys_pipe, nums, nums_length, H, outfile);

    // get result
    read(max_pipe[0], max, sizeof(*max));
    read(avg_pipe[0], avg, sizeof(*avg));
    read(keys_pipe[0], keys, sizeof(*keys));

    // output result
    fprintf(outfile, "Max=%d, Avg=%lf, Keys=%d \n", *max, *avg, *keys);
    fflush(outfile);

    // free array
    al_free(&data);

    return 0;
}