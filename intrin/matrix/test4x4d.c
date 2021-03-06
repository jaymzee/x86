#include "matrix4x4d.h"
#include "print_avx.h"
#include <stdio.h>

__m256d a[4] = {
    { 1,  2,  3,  4},
    { 5,  6,  7,  8},
    { 9,  3,  2,  1},
    { 5,  4,  7,  6}
};

__m256d y, x = {1, 2, 3, 4};

int main()
{
    int i;

    for (i = 0; i < 10000; i++) {
        y = m4x4v_cd(a, x);
    }

    y = m4x4v_cd(a, x);
    print_m256d(y);
    y = m4x4v_rd(a, x);
    print_m256d(y);

    printf("success!\n");

    return 0;
}

