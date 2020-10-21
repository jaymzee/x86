#include <stdio.h>
#include <immintrin.h>

void print_m256d(__m256d x)
{
    double p[4];

    // more complex than just accessing x[j] but it's MSVC compatible
    _mm256_storeu_pd(p, x) ;

    printf("[");
    for (int j = 0; j < 4; j++) {
        if (j > 0) {
            printf(" ");
        }
        printf("%g", p[j]);
    }
    printf("]\n");
}
