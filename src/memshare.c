#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <pthread.h>

#include "memshare.h"

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS 33
#endif

counter_sync_t * csync_init() {
    counter_sync_t * counter_ptr = init_memshare(sizeof(counter_sync_t));
    counter_ptr->counter = 0;
    pthread_mutex_init(&counter_ptr->lock, NULL);
    return counter_ptr;
}

void csync_increment(counter_sync_t * counter) {
    // We want to lock processes while updating this count variable
    pthread_mutex_lock(&counter->lock);
    counter->counter++;
    pthread_mutex_unlock(&counter->lock);
}

int csync_view(counter_sync_t * counter) {
    pthread_mutex_lock(&counter->lock);
    int value = counter->counter;
    pthread_mutex_unlock(&counter->lock);
    return value;
}

int csync_destroy(counter_sync_t * counter) {
    return munmap(counter, sizeof(counter_sync_t));
}

void * init_memshare(size_t size) {
    int prot = PROT_READ | PROT_WRITE;
    int vis = MAP_SHARED | MAP_ANONYMOUS;
    return mmap(NULL, size, prot, vis, -1, 0);
}