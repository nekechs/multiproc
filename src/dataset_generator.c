#include <stdio.h>
#include <stdlib.h>

#include "dataset_generator.h"
#include "random.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int generate_file(const char * path, unsigned int num_keys, unsigned int num_total, unsigned int max) {
    FILE * ptr = fopen(path, "w");
    char str_buf[64];

    if(!ptr) {
        perror("generate_file could not generate a new file specified by char * path.");
        // exit(EXIT_FAILURE);
        return -1;
    }

    const double percentage = ((double)num_keys) / num_total;

    if(DEBUG) {
        printf("%f\n", percentage);
    }

    unsigned int keys_left = num_keys;
    for(int i = 0; i < num_total - keys_left - 1; i++) {
        if(keys_left && (rand_fp() < percentage)) {
            // Here, we want to insert a hidden key into the file.
            keys_left--;
            sprintf(str_buf, "%d\n", -1);
        } else {
            // Here, we want to insert a pos normal number into the file.
            int num = rand_fp() * max;
            sprintf(str_buf, "%d\n", num);
        }
        fputs(str_buf, ptr);
    }

    // Any remaining keys, we just append.
    for(int i = keys_left; i >= 0; i--) {
        fputs("-1\n", ptr);
    }

    fclose(ptr);
    return 0;
}