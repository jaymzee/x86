#include <immintrin.h>
#include "matrix.h"

__m128 m4x4v_cs_SSE(const __m128 cols[4], const __m128 x) {
    // x[0] * col[0] + x[1] * col[1] + x[2] * col[2] + x[3] * col[3]
  __m128 u0 = _mm_shuffle_ps(x,x, _MM_SHUFFLE(0,0,0,0));
  __m128 u1 = _mm_shuffle_ps(x,x, _MM_SHUFFLE(1,1,1,1));
  __m128 u2 = _mm_shuffle_ps(x,x, _MM_SHUFFLE(2,2,2,2));
  __m128 u3 = _mm_shuffle_ps(x,x, _MM_SHUFFLE(3,3,3,3));

  __m128 v0 = _mm_mul_ps(u0, cols[0]);
  __m128 v1 = _mm_mul_ps(u1, cols[1]);
  __m128 v2 = _mm_mul_ps(u2, cols[2]);
  __m128 v3 = _mm_mul_ps(u3, cols[3]);

  return _mm_add_ps(_mm_add_ps(v0, v1), _mm_add_ps(v2, v3));
}

__m128 m4x4v_rs_SSE3(const __m128 rows[4], const __m128 x) {
  __m128 v0 = _mm_mul_ps(rows[0], x);
  __m128 v1 = _mm_mul_ps(rows[1], x);
  __m128 v2 = _mm_mul_ps(rows[2], x);
  __m128 v3 = _mm_mul_ps(rows[3], x);

  return _mm_hadd_ps(_mm_hadd_ps(v0, v1), _mm_hadd_ps(v2, v3));
}

__m128 m4x4v_rs_SSE4(const __m128 rows[4], const __m128 x) {
  __m128 v0 = _mm_dp_ps (rows[0], x, 0xFF);
  __m128 v1 = _mm_dp_ps (rows[1], x, 0xFF);
  __m128 v2 = _mm_dp_ps (rows[2], x, 0xFF);
  __m128 v3 = _mm_dp_ps (rows[3], x, 0xFF);

  return _mm_shuffle_ps(_mm_movelh_ps(v0, v1), _mm_movelh_ps(v2, v3),  _MM_SHUFFLE(2, 0, 2, 0));
}  
