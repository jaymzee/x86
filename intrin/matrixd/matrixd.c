#include <immintrin.h>
#include "matrixd.h"

__m256d m4x4v_rowSSE3(const __m256d rows[4], const __m256d x) {
  __m256d v0 = _mm256_mul_pd(rows[0], x);
  __m256d v1 = _mm256_mul_pd(rows[1], x);
  __m256d v2 = _mm256_mul_pd(rows[2], x);
  __m256d v3 = _mm256_mul_pd(rows[3], x);

  return _mm256_hadd_pd(_mm256_hadd_pd(v0, v1), _mm256_hadd_pd(v2, v3));
}

