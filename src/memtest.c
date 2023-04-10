#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "memshare.h"

int main() {
    counter_sync_t * counter = csync_init();

    pid_t pid = fork();
    if(pid > 0) {
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        csync_increment(counter);
        wait(NULL);
    } else {
        // sleep(1);
        printf("%d\n", csync_view(counter));
        printf("%d\n", csync_view(counter));
        printf("%d\n", csync_view(counter));
        sleep(1);
        printf("%d\n", csync_view(counter));
    }

    return 0;
}