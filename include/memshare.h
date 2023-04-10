#ifndef _MEM_SHARE_H_
#define _MEM_SHARE_H_

#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int counter;
    pthread_mutex_t lock;
} counter_sync_t;

/* Initializes a counter variable (in SHARED memory using mmap) */
counter_sync_t * csync_init();

/* Increments the shared memory count (and mutually excludes everything else while updating) */
void csync_increment(counter_sync_t * counter);

/* Returns the count value (mutually excludes everything else while viewing)*/
int csync_view(counter_sync_t * counter);

/* Deallocates the counter and returns the result of munmap */
int csync_destroy(counter_sync_t * counter);

/* Initializes some shared memory that a parent can share with children. */
void * init_memshare(size_t size);



#endif