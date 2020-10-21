#include <immintrin.h>

__m128 m4x4v_cs_SSE(const __m128 cols[4], const __m128 v);
__m128 m4x4v_rs_SSE3(const __m128 rows[4], const __m128 v);
__m128 m4x4v_rs_SSE4(const __m128 rows[4], const __m128 v);
