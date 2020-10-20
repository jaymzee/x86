#include <stdio.h>
#include <immintrin.h>
#include "matrixd.h"

double mul_sd(double x, double y)
{
    __m128d xx = _mm_set_sd(x);
    __m128d yy = _mm_set_sd(y);
    return _mm_mul_sd(xx, yy)[0];
}

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
        {16,  2,  3, 13},
        { 5, 11, 10,  8},
        { 9,  7,  6, 12},
        { 4, 14, 15,  1}
    };
    __m256d y, x = {1, 2, 3, 4};

    for (int i = 0; i < 10000000; i++) {
        y = m4x4v_rowSSE3(a, x);
    }

    y = m4x4v_rowSSE3(a, x);
    print_vector(y);
}

