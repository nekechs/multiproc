#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "bfs_solver_signals.h"
#include "arraylist.h"
#include <signal.h>

void handle_signal(int signum)
{
    // do nothing, just return from signal handler
    printf("Received signal %d\n", signum);
}

void fork_tree(int n, int id, int *max_pipe, int *avg_pipe, int *keys_pipe, int *nums, int nums_length, int H, FILE *outfile)
{

    // printf("Process with ID %d created\n", id);
    fprintf(outfile, "Hi, I am process %d with return arg %d, and my parent is %d.\n", getpid(), id, getppid());
    fflush(outfile);

    // fork left child
    int max_pipe_l[2];
    int avg_pipe_l[2];
    int keys_pipe_l[2];
    pid_t left_child_pid;
    if (id * 2 + 1 < n)
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
    if (id * 2 + 2 < n)
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
    int num_keys = 0;
    for (int i = s_i; i < e_i; i++)
    {
        if (nums[i] == -1)
        {
            num_keys += 1;
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

    // wait for left child
    int local_max_left = -1;
    int num_keys_left = -1;
    if (id * 2 + 1 < n)
    {
        int max_l;
        double avg_l;
        int keys_l;

        read(max_pipe_l[0], &max_l, sizeof(max_l));
        read(avg_pipe_l[0], &avg_l, sizeof(avg_l));
        read(keys_pipe_l[0], &keys_l, sizeof(keys_l));

        if (max_l > max)
        {
            max = max_l;
            local_max_left = max_l;
        }

        avg += avg_l;
        num_keys += keys_l;
        num_keys_left = keys_l;
    }

    // wait for right child
    int local_max_right = -1;
    int num_keys_right = -1;
    if (id * 2 + 2 < n)
    {
        int max_r;
        double avg_r;
        int keys_r;

        read(max_pipe_r[0], &max_r, sizeof(max_r));
        read(avg_pipe_r[0], &avg_r, sizeof(avg_r));
        read(keys_pipe_r[0], &keys_r, sizeof(keys_r));

        if (max_r > max)
        {
            max = max_r;
            local_max_right = max_r;
        }

        avg += avg_r;
        num_keys += keys_r;
        num_keys_right = keys_r;
    }

    // if localMax(child) > localMax(Parent) or localMAX(child) > localMAXs, then send signal to continue
    if (local_max_left != -1 && local_max_right != -1)
    {
        if (local_max_left > max)
        {
            signal(left_child_pid, SIGCONT);
        }
        else if (num_keys_left != -1 && num_keys_left > H)
        {
            signal(left_child_pid, SIGKILL);
        }
        if (local_max_right > max)
        {
            signal(left_child_pid, SIGCONT);
        }
        else if (num_keys_right != -1)
        {
            signal(right_child_pid, SIGKILL);
        }
    }
    // write result to pipes
    write(max_pipe[1], &max, sizeof(max));
    write(avg_pipe[1], &avg, sizeof(avg));
    write(keys_pipe[1], &num_keys, sizeof(num_keys));
    // exit if not main parent
    if (id != 0)
    {
        // pause child process
        signal(SIGTSTP, handle_signal);
        raise(SIGTSTP);

        sleep(1000);
        _exit(id);
    }
}

int solve_bfs_signals(char *filename, int *max, double *avg, int *keys, int H, int pn, FILE *outfile)
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
