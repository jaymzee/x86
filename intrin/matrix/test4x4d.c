#include <stdio.h>
#include <immintrin.h>
#include "matrix4x4d.h"

void print_vector(__m256d x)
{
    printf("[ ");
    for (int j = 0; j < 4; j++) {
        printf("%g ", x[j]);
    }
    printf("]\n");
}

int main()
{
    __m256d a[4] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9,  3,  2,  1},
        { 5,  4,  7,  6}
    };
    __m256d y, x = {1, 2, 3, 4};

    for (int i = 0; i < 10000000; i++) {
        y = m4x4v_rd(a, x);
    }

    y = m4x4v_cd(a, x);
    print_vector(y);
    y = m4x4v_rd(a, x);
    print_vector(y);
}

