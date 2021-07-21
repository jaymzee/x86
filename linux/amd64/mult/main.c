#include <stdio.h>
#include <stdlib.h>
#include "mult.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: mult x y\n");
        exit(1);
    }

    double x = atof(argv[1]);
    double y = atof(argv[2]);
    printf("x * y = %g\n", mult(x, y));
}
