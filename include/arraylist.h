#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

#include <stdlib.h>

typedef struct {
    char * data;
    size_t elemsize_bytes;
    size_t nmemb_max;
    size_t nmemb;
} arraylist_t;

/**
 * Given a pointer to an arraylist_t object already in memory and starting params,
 * initializes an arraylist using the specified element size and maximum capacity.
 * 
 * Note: Assume that A points to a location in memory that already has space for
 * the arraylist_t (not the actual data itself)
 */
void al_alloc(arraylist_t * A, size_t elemsize, size_t nmemb_max);

/**
 * Given a pointer to an arraylist_t that has already been allocated on the stack,
 * free the memory of arraylist_t 
*/
void al_free(arraylist_t * A);

/**
 * Inserts data into the arraylist so that it will occupy the specified index. Moves
 * data around if the index is not the last element of the list. If the capacity is reached,
 * a resize will take place.
*/
void al_insert(arraylist_t * A, size_t index, const void * elem_data);

/**
 * Removes an element from the arraylist given an index. May move some elements around.
*/
void al_remove(arraylist_t * A, size_t index);

/**
 * Retrieves an element and copies over the data into dst.
 * 
 * If invalid access, returns -1. Returns 0 on success.
*/
int al_get(arraylist_t * A, size_t index, void * dst);

#endif