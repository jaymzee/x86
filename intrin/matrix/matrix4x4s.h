#ifndef MATRIX4X4S_H
#define MATRIX4X4S_H

#include <emmintrin.h>

__m128 m4x4v_cs_SSE(const __m128 cols[4], const __m128 v);
__m128 m4x4v_rs_SSE3(const __m128 rows[4], const __m128 v);
__m128 m4x4v_rs_SSE4(const __m128 rows[4], const __m128 v);

#endif /* MATRIX4X4S_H */
