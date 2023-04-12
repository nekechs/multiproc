#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "arraylist.h"

void al_alloc(arraylist_t * A, size_t elemsize, size_t nmemb_max) {
    A->data = malloc(elemsize * nmemb_max);
    A->elemsize_bytes = elemsize;
    A->nmemb = 0;
    A->nmemb_max = nmemb_max;
}

void al_free(arraylist_t * A) {
    free(A->data);
    A->elemsize_bytes = 0;
    A->nmemb = 0;
    A->nmemb_max = 0;
}

void al_insert(arraylist_t * A, size_t index, const void * elem_data) {
    if(index > A->nmemb) {
        /* Problem: the index for insertion is out of bounds. */
        index = A->nmemb;
    }

    if(A->nmemb == A->nmemb_max) {
        /* We are at capacity; We must resize. */
        char * old_data = A->data;
        A->data = malloc(2 * A->nmemb_max * A->elemsize_bytes);
        memcpy(A->data, old_data, A->nmemb_max * A->elemsize_bytes);
        A->nmemb_max *= 2;
        free(old_data);
    }

    char * data = A->data;
    const size_t size_bytes = A->elemsize_bytes;

    /* Move the data 1 element's worth to the right. */
    memmove(data + (index + 1) * size_bytes, data + index * size_bytes, size_bytes * (A->nmemb - index));

    /* Place the element into the appropriate place in the array. */
    memcpy(data + size_bytes * index, elem_data, size_bytes);

    A->nmemb++;
}

void al_remove(arraylist_t * A, size_t index) {
    if(index >= A->nmemb) {
        index = A->nmemb - 1;
    }

    char * data = A->data;
    const size_t size_bytes = A->elemsize_bytes;

    /* All we really need to do is shift elements left. */
    memmove(data + index * size_bytes, data + (index + 1) * size_bytes, size_bytes * (A->nmemb - 1 - index));
    A->nmemb--;
}

int al_get(arraylist_t * A, size_t index, void * dst) {
    if(index >= A->nmemb) {
        return -1;
    }
    
    memcpy(dst, A->data + index * A->elemsize_bytes, A->elemsize_bytes);
    return 0;
}