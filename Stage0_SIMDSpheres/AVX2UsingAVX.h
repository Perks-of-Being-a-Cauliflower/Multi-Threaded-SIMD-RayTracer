// definitions of some AVX2 intrinsics using just AVX1 instructions
// this is far from an exhaustive implementation, but should hopefully be enough for the assignment

#ifndef _AVX2USINGAVX
#define _AVX2USINGAVX

#ifndef __AVX2__

#include <immintrin.h>

#define REPLACE1i(INTRINSIC256, INTRINSIC128) \
__forceinline __m256i _##INTRINSIC256(__m256i a) \
{ \
	__m256i ret; \
	__m128i* as = (__m128i*) &a; \
	__m128i* rets = (__m128i*) &ret; \
	rets[0] = INTRINSIC128(as[0]); \
	rets[1] = INTRINSIC128(as[1]); \
	return ret; \
}

#define REPLACE2i(INTRINSIC256, INTRINSIC128) \
__forceinline __m256i _##INTRINSIC256(__m256i a, __m256i b) \
{ \
	__m256i ret; \
	__m128i* as = (__m128i*) &a; \
	__m128i* bs = (__m128i*) &b; \
	__m128i* rets = (__m128i*) &ret; \
	rets[0] = INTRINSIC128(as[0], bs[0]); \
	rets[1] = INTRINSIC128(as[1], bs[1]); \
	return ret; \
}

#define _mm256_min_epu32 __mm256_min_epu32
REPLACE2i(_mm256_min_epu32, _mm_min_epu32)

#define _mm256_max_epu32 __mm256_max_epu32
REPLACE2i(_mm256_max_epu32, _mm_max_epu32)

#define _mm256_and_si256 __mm256_and_si256
REPLACE2i(_mm256_and_si256, _mm_and_si128)

#define _mm256_andnot_si256 __mm256_andnot_si256
REPLACE2i(_mm256_andnot_si256, _mm_andnot_si128)

#define _mm256_or_si256 __mm256_or_si256
REPLACE2i(_mm256_or_si256, _mm_or_si128)

#define _mm256_cmpgt_epi32 __mm256_cmpgt_epi32
REPLACE2i(_mm256_cmpgt_epi32, _mm_cmpgt_epi32)

#define _mm256_cmpeq_epi32 __mm256_cmpeq_epi32
REPLACE2i(_mm256_cmpeq_epi32, _mm_cmpeq_epi32)

#define _mm256_add_epi32 __mm256_add_epi32
REPLACE2i(_mm256_add_epi32, _mm_add_epi32)

#define _mm256_sub_epi32 __mm256_sub_epi32
REPLACE2i(_mm256_sub_epi32, _mm_sub_epi32)

#define _mm256_mullo_epi32 __mm256_mullo_epi32
REPLACE2i(_mm256_mullo_epi32, _mm_mullo_epi32)

#define _mm256_abs_epi32 __mm256_abs_epi32
REPLACE1i(_mm256_abs_epi32, _mm_abs_epi32)

//#define _mm256_hadd_epi32 __mm256_hadd_epi32 // not confident about this one
//REPLACE2i(_mm256_hadd_epi32, _mm_hadd_epi32)

#endif

#endif