#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "dfs_solver.h"
#include "dfs_solver_signal.h"
#include "arraylist.h"

#ifndef DEBUG
#define DEBUG 1
#endif

#define BUF_SIZE 256

struct dfs_chunk dfs_chunk_merge(const struct dfs_chunk * left, const struct dfs_chunk * right) {
    struct dfs_chunk result;
    result.max = left->max > right->max ? left->max : right-> max;      // Grab the maximum value of the combined set.
    result.nmemb = left->nmemb + right->nmemb;                          // Add to find total number combined in both sets
    result.mean = (left->mean * left->nmemb + right->mean * right->nmemb) / result.nmemb;
    result.num_key = left->num_key + right->num_key;

    return result;
}

int solve_dfs_signal(char * filename, int * max, double * avg, int num_proc, FILE * outfile) {
    /* First, we need to read all of the elements into an arraylist_t, and store the number of elements. */
    FILE * ptr = fopen(filename, "r");
    if(!ptr) {
        /* This has been a failure. Error reading file. */
        perror("Error when trying to read input file.");
        return -1;
    }

    /* This is the list that will store all of our integers. */
    arraylist_t A;
    al_alloc(&A, sizeof(int), 32);
    int num_read = 0;

    char buffer[BUF_SIZE];
    char * s = buffer;

    s = fgets(buffer, BUF_SIZE, ptr);
    while(s) {
        int current = atoi(s);
        al_insert(&A, A.nmemb, &current);
        s = fgets(buffer, BUF_SIZE, ptr);
        num_read++;
    }

    /* If the number of integers is less than the number of processes.. Don't need to spawn more processes than needed!*/
    if(A.nmemb < num_proc) {
        num_proc = A.nmemb;
    }
    
    /* Pipe initialization */
    int * fd_list = malloc(2 * num_proc * sizeof(int));
    for(int i = 0; i < num_proc; i++) {
        int res = pipe(fd_list + 2*i);
    }

    

    int pn;
    for(pn = 0; pn < num_proc; pn++) {
        pid_t pid = fork();
        if(pid > 0) {
            fprintf(outfile, "Hi, I am process %d with return arg %d, and my parent is %d.\n", getpid(), pn, getppid());
            wait(NULL);
            break;
        } else if(pid < 0) {
            // BIG ERROR
            // TODO: FINISH
        }

        /* We are in a newly created child */
        int lower = (pn * A.nmemb) / num_proc;
        int upper = ( (pn+1) * A.nmemb) / num_proc;

        int max = INT_MIN;
        long int sum = 0;
        int num_key = 0;


        for(int i = lower; i < upper; i++) {
            int num;
            al_get(&A, i, &num);

            if(num > max) max = num;
            if(num == -1) {
                num_key++;
                fprintf(outfile, "Hi, I am process %d with return arg %d. I found the hidden key in position A[%d]\n", getpid(), pn, i);
            }
            sum += num;
        }

        struct dfs_chunk answer;
        answer.nmemb = upper - lower;
        answer.max = max;
        answer.mean = ((double)sum) / answer.nmemb;
        answer.num_key = num_key;
        int bytes_written = write(*(fd_list + 2*pn + 1), &answer, sizeof(struct dfs_chunk));
    }
    // write(fd[1], &answer, sizeof(struct dfs_chunk));
    /* Now the process needs to do actual work. */

    
    /* Now, we compute the avg. and max for this chunk. */
    // printf("%d\n", pn);
    al_free(&A);

    struct dfs_chunk final_ans;
    if(pn == 0) {
        struct dfs_chunk acc = {
            .nmemb = 0,
            .mean = 0,
            .max = INT_MIN,
            .num_key = 0
        };
        struct dfs_chunk current;
        for(int i = 0; i < num_proc; i++) {
            int x;
            int bytes_read = read(*(fd_list + 2*i), &current, sizeof(struct dfs_chunk));
            acc = dfs_chunk_merge(&acc, &current);
        }
        final_ans = acc;
    }

    free(fd_list);

    if(pn == 0 && max && avg) {
        *max = final_ans.max;
        *avg = final_ans.mean;
        // printf("%lf\n", *avg);
        fprintf(outfile, "Max=%d, Avg=%lf\n", final_ans.max, final_ans.mean);
        return final_ans.num_key;
    }

    exit(pn);
}