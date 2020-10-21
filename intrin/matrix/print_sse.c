#include <stdio.h>
#include <emmintrin.h>

void print_m128(__m128 x)
{
    float p[4];

    _mm_storeu_ps(p, x);

    printf("[");
    for (int j = 0; j < 4; j++) {
        if (j > 0) {
            printf(" ");
        }
        printf("%g", p[j]);
    }
    printf("]\n");
}
