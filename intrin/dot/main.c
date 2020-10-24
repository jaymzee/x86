#include <stdio.h>
#include <immintrin.h>

// intrinsics for taking dot product of dim 2, 3, or 4 vectors
// requires SSE4

int main()
{
    __m128 u = {1.0, 2.0, 3.0, 4.0};
    __m128 v = {0.3, 0.6, 0.1, 0.7};
    __m128 w;

    w = _mm_dp_ps(u, v, 0xf1);

    // more cumbersome than w[0] but MSVC compatible
    float p[4];
    _mm_storeu_ps(p, w);
    printf("%g\n", p[0]);

    return 0;
}
