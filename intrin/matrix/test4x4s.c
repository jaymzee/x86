#include "matrix4x4s.h"
#include "print_sse.h"

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
        //y = m4x4v_rs_SSE3(a, x);
        //y = m4x4v_rs_SSE4(a, x);
    }

    y = m4x4v_cs_SSE(a, x);
    print_m128(y);
    y = m4x4v_rs_SSE3(a, x);
    print_m128(y);
    y = m4x4v_rs_SSE4(a, x);
    print_m128(y);
}

