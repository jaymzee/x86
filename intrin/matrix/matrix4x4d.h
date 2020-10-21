#ifndef MATRIX4X4D_H
#define MATRIX4X4D_H

#include <immintrin.h>

__m256d m4x4v_cd(const __m256d cols[4], __m256d v);
__m256d m4x4v_rd(const __m256d rows[4], __m256d v);

#endif /* MATRIX4X4D_H */
