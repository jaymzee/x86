#include <immintrin.h>

__m128 m4x4v_colSSE(const __m128 cols[4], const __m128 v);
__m128 m4x4v_rowSSE3(const __m128 rows[4], const __m128 v);
__m128 m4x4v_rowSSE4(const __m128 rows[4], const __m128 v);
