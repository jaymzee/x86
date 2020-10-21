#include <stdio.h>
#include <emmintrin.h>
#include "matrix4x4s.h"

void print_vector(__m128 x)
{
    printf("[ ");
    for (int j = 0; j < 4; j++) {
        printf("%g ", x[j]);
    }
    printf("]\n");
}

int main()
{
    __m128 a[4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 3, 2, 1},
        {5, 4, 7, 6}
    };
    __m128 y, x = {1, 2, 3, 4};

    for (int i = 0; i < 10000000; i++) {
        y = m4x4v_cs_SSE(a, x);
        //y = m4x4v_rowSSE3(a, x);
        //y = m4x4v_rowSSE4(a, x);
    }
    y = m4x4v_cs_SSE(a, x);
    print_vector(y);
    y = m4x4v_rs_SSE3(a, x);
    print_vector(y);
    y = m4x4v_rs_SSE4(a, x);
    print_vector(y);
}

