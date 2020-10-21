#include <stdio.h>
#include <immintrin.h>

void print_m256d(__m256d x)
{
    printf("[");
    for (int j = 0; j < 4; j++) {
        if (j > 0) {
            printf(" ");
        }
        printf("%g", x[j]);
    }
    printf("]\n");
}
