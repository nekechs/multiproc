#include <stdio.h>
#include <stdlib.h>

#include "dataset_generator.h"
#include "random.h"

int generate_file(const char * path, unsigned int num_keys, unsigned int num_total, unsigned int max) {
    FILE * ptr = fopen(path, "w");

    if(!ptr) {
        perror("generate_file could not generate a new file specified by char * path.");
        exit(EXIT_FAILURE);
    }

    const double percentage = ((double)num_keys) / num_total;

    unsigned int keys_left = num_keys;
    for(int i = 0; i < num_keys; i++) {
        if(keys_left && rand_fp() < percentage) {
            // Here, we want to insert a hidden key into the file.
            keys_left--;
            putw(-1, ptr);
        } else {
            // Here, we want to insert a pos normal number into the file.
            int num = rand_fp() * max;
            putw(num, ptr);
        }
    }

    // Any remaining keys, we just append.
    for(int i = keys_left; i >= 0; i--) {
        putw(-1, ptr);
    }

    fclose(ptr);
}