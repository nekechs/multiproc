#include <stdio.h>
#include "dataset_generator.h"
#include "arraylist.h"

int main() {
    generate_file("./xd.txt", 20, 1000, 500);
    printf("bruh\n");

    arraylist_t A;
    al_alloc(&A, sizeof(int), 1);

    int x = 8;
    al_insert(&A, A.nmemb, &x);
    al_insert(&A, A.nmemb, &x);
    al_insert(&A, A.nmemb, &x);

    int * arr = A.data;
    for(int i = 0; i < 3; i++) {
        printf("%d\n", arr[i]);
    }

    al_free(&A);
}