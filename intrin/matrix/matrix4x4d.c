#include "matrix4x4d.h"

__m256d m4x4v_cd(const __m256d cols[static 4], const __m256d x) {
    // AVX2
    // matrix vector product (column major)
    // x[0] * col[0] + x[1] * col[1] + x[2] * col[2] + x[3] * col[3]
  __m256d u0 = _mm256_permute4x64_pd(x, _MM_SHUFFLE(0,0,0,0));
  __m256d u1 = _mm256_permute4x64_pd(x, _MM_SHUFFLE(1,1,1,1));
  __m256d u2 = _mm256_permute4x64_pd(x, _MM_SHUFFLE(2,2,2,2));
  __m256d u3 = _mm256_permute4x64_pd(x, _MM_SHUFFLE(3,3,3,3));

  __m256d v0 = _mm256_mul_pd(u0, cols[0]);
  __m256d v1 = _mm256_mul_pd(u1, cols[1]);
  __m256d v2 = _mm256_mul_pd(u2, cols[2]);
  __m256d v3 = _mm256_mul_pd(u3, cols[3]);

  return _mm256_add_pd(_mm256_add_pd(v0, v1), _mm256_add_pd(v2, v3));
}

__m256d m4x4v_rd(const __m256d rows[static 4], const __m256d x) {
    // AVX
    // matrix vector product (row major)
  __m256d v0 = _mm256_mul_pd(rows[0], x);
  __m256d v1 = _mm256_mul_pd(rows[1], x);
  __m256d v2 = _mm256_mul_pd(rows[2], x);
  __m256d v3 = _mm256_mul_pd(rows[3], x);

  __m256d u0 = _mm256_permute4x64_pd(_mm256_hadd_pd(v0, v1), 0xd8);
  __m256d u1 = _mm256_permute4x64_pd(_mm256_hadd_pd(v2, v3), 0xd8);

  return _mm256_permute4x64_pd(_mm256_hadd_pd(u0, u1), 0xd8);
}

