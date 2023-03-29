#ifndef _DATASET_GENERATOR_H_
#define _DATASET_GENERATOR_H_

// Generate the file that we want with the filename specified by "path" and the other params just look at it bruh
// Returns 0 on success, -1 on fail'd
int generate_file(const char * path, unsigned int num_keys, unsigned int num_total, unsigned int max);

#endif