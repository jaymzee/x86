#include <stdio.h>
#include <immintrin.h>

int main(int argc, char *argv[])
{
    __m128 u = {1.0, 2.0, 3.0, 4.0};
    __m128 v = {0.3, 0.6, 0.1, 0.7};
    __m128 w;

    w = _mm_dp_ps(u, v, 0xf1);

    printf("%g\n", w[0]);
}
