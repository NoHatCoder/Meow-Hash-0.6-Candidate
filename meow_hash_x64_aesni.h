
#if !defined(MEOW_HASH_X64_AESNI_H)

#define MEOW_HASH_VERSION 6
#define MEOW_HASH_VERSION_NAME "0.6/bodie"

#if !defined(meow_u8)

#if _MSC_VER
#include <intrin.h>
#define meow_msc_align __declspec(align(16))
#define meow_gcc_align
#else
#include <x86intrin.h>
#define meow_msc_align
#define meow_gcc_align __attribute__((aligned(16)))
#endif

#define meow_u8 char unsigned
#define meow_u64 long long unsigned
#define meow_u128 __m128i

#endif

#define meow_xor(A,B) _mm_xor_si128(A,B)
#define meow_add(A,B) _mm_add_epi8(A,B)
#define meow_sub(A,B) _mm_sub_epi8(A,B)
#define meow_aesenc(A,B,C) _mm_aesenc_si128(_mm_xor_si128(A,B),C)
#define meow_load(A) _mm_loadu_si128((__m128i *)(A))
#define meow_store(A,B) _mm_storeu_si128((__m128i *)(A),B)

/*
//Old complexity 5 pattern
#define MEOW_MIX1(ptr) \
{ \
meow_u128 ttt0,ttt1,ttt2,ttt3; \
ttt0=meow_add(xmm6,meow_load(ptr+0x10)); \
ttt1=meow_aesenc(xmm4,meow_load(ptr+0x00),xmm1); \
xmm6=meow_add(xmm2,meow_load(ptr+0x00)); \
ttt2=meow_aesenc(xmm1,meow_load(ptr+0x10),xmm5); \
ttt3=meow_add(xmm7,meow_load(ptr+0x30)); \
xmm1=meow_aesenc(xmm5,meow_load(ptr+0x20),xmm6); \
xmm7=meow_add(xmm3,meow_load(ptr+0x20)); \
xmm6=meow_aesenc(xmm6,meow_load(ptr+0x30),ttt0); \
xmm4=meow_add(xmm0,meow_load(ptr+0x50)); \
xmm2=meow_aesenc(ttt0,meow_load(ptr+0x40),xmm7); \
xmm0=meow_add(ttt1,meow_load(ptr+0x40)); \
xmm7=meow_aesenc(xmm7,meow_load(ptr+0x50),ttt3); \
xmm5=meow_add(ttt2,meow_load(ptr+0x70)); \
xmm3=meow_aesenc(ttt3,meow_load(ptr+0x60),xmm0); \
xmm1=meow_add(xmm1,meow_load(ptr+0x60)); \
xmm0=meow_aesenc(xmm0,meow_load(ptr+0x70),xmm4); \
}

#define MEOW_SHUFFLE1() \
{ \
meow_u128 ttt0; \
meow_u64 a; \
for(a=0;a<4;a++){ \
xmm1=meow_add(xmm1,xmm5); \
xmm4=meow_aesenc(xmm4,xmm6,xmm1); \
xmm5=meow_add(xmm5,xmm6); \
xmm1=meow_aesenc(xmm1,xmm2,xmm5); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=ttt0; \
} \
}
*/

#ifdef MEOW_DEF
#define MEOW_MIX1(ptr) \
{ \
meow_u128 ttt0; \
meow_u64 a; \
for(a=0;a<4;a++){ \
xmm1=meow_add(xmm1,meow_load(ptr+0x10+a*0x20)); \
xmm0=meow_aesenc(xmm0,meow_load(ptr+0x00+a*0x20),xmm2); \
xmm2=meow_add(xmm2,meow_load(ptr+0x00+a*0x20)); \
xmm5=meow_aesenc(xmm5,meow_load(ptr+0x10+a*0x20),xmm2); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=ttt0; \
} \
}
#else
#define MEOW_MIX1(ptr) \
{ \
meow_u128 ttt0,ttt1,ttt2; \
ttt0=meow_add(xmm1,meow_load(ptr+0x10)); \
ttt2=meow_aesenc(xmm0,meow_load(ptr+0x00),xmm2); \
xmm2=meow_add(xmm2,meow_load(ptr+0x00)); \
xmm1=meow_aesenc(xmm5,meow_load(ptr+0x10),xmm2); \
ttt1=meow_add(xmm2,meow_load(ptr+0x30)); \
xmm5=meow_aesenc(ttt0,meow_load(ptr+0x20),xmm3); \
xmm3=meow_add(xmm3,meow_load(ptr+0x20)); \
xmm2=meow_aesenc(xmm6,meow_load(ptr+0x30),xmm3); \
ttt0=meow_add(xmm3,meow_load(ptr+0x50)); \
xmm6=meow_aesenc(ttt1,meow_load(ptr+0x40),xmm4); \
xmm4=meow_add(xmm4,meow_load(ptr+0x40)); \
xmm3=meow_aesenc(xmm7,meow_load(ptr+0x50),xmm4); \
xmm0=meow_add(xmm4,meow_load(ptr+0x70)); \
xmm7=meow_aesenc(ttt0,meow_load(ptr+0x60),xmm1); \
xmm1=meow_add(xmm1,meow_load(ptr+0x60)); \
xmm4=meow_aesenc(ttt2,meow_load(ptr+0x70),xmm1); \
}
#endif

#ifdef MEOW_DEF
#define MEOW_SHUFFLE1() \
{ \
meow_u128 ttt0; \
meow_u64 a; \
for(a=0;a<4;a++){ \
xmm5=meow_add(xmm5,xmm1); \
xmm0=meow_aesenc(xmm0,xmm2,xmm5); \
xmm1=meow_add(xmm1,xmm2); \
xmm5=meow_aesenc(xmm5,xmm6,xmm1); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=ttt0; \
} \
}
#else
#define MEOW_SHUFFLE1() \
{ \
meow_u128 ttt0; \
ttt0=meow_add(xmm5,xmm1); \
xmm0=meow_aesenc(xmm0,xmm2,ttt0); \
xmm5=meow_add(xmm1,xmm2); \
xmm1=meow_aesenc(ttt0,xmm6,xmm5); \
ttt0=meow_add(xmm6,xmm2); \
xmm5=meow_aesenc(xmm5,xmm3,ttt0); \
xmm6=meow_add(xmm2,xmm3); \
xmm2=meow_aesenc(ttt0,xmm7,xmm6); \
ttt0=meow_add(xmm7,xmm3); \
xmm6=meow_aesenc(xmm6,xmm4,ttt0); \
xmm7=meow_add(xmm3,xmm4); \
xmm3=meow_aesenc(ttt0,xmm0,xmm7); \
ttt0=meow_add(xmm0,xmm4); \
xmm7=meow_aesenc(xmm7,xmm1,ttt0); \
xmm0=meow_add(xmm4,xmm1); \
xmm4=meow_aesenc(ttt0,xmm5,xmm0); \
}
#endif

#ifdef MEOW_DEF
#define MEOW_MIX2(ptr) \
{ \
meow_u128 ttt0; \
meow_u64 a; \
for(a=0;a<4;a++){ \
xmm0=meow_aesenc(xmm0,meow_load(ptr+0x00+a*0x20),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr+0x10+a*0x20),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr+0x10+a*0x20),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr+0x00+a*0x20)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
} \
}
#else
#define MEOW_MIX2(ptr) \
{ \
meow_u128 ttt0; \
ttt0=meow_aesenc(xmm0,meow_load(ptr+0x00),xmm4); \
xmm0=meow_aesenc(xmm4,meow_load(ptr+0x10),xmm8); \
xmm4=meow_aesenc(xmm8,meow_load(ptr+0x10),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr+0x00)); \
xmm8=meow_aesenc(xmm1,meow_load(ptr+0x20),xmm5); \
xmm1=meow_aesenc(xmm5,meow_load(ptr+0x30),xmm9); \
xmm5=meow_aesenc(xmm9,meow_load(ptr+0x30),xmm2); \
ttt0=meow_add(ttt0,meow_load(ptr+0x20)); \
xmm9=meow_aesenc(xmm2,meow_load(ptr+0x40),xmm6); \
xmm2=meow_aesenc(xmm6,meow_load(ptr+0x50),xmm10); \
xmm6=meow_aesenc(xmm10,meow_load(ptr+0x50),xmm3); \
xmm8=meow_add(xmm8,meow_load(ptr+0x40)); \
xmm10=meow_aesenc(xmm3,meow_load(ptr+0x60),xmm7); \
xmm3=meow_aesenc(xmm7,meow_load(ptr+0x70),ttt0); \
xmm7=meow_aesenc(ttt0,meow_load(ptr+0x70),xmm0); \
xmm9=meow_add(xmm9,meow_load(ptr+0x60)); \
}
#endif

#ifdef MEOW_DEF
#define MEOW_MIX2S(ptr) \
{ \
meow_u128 ttt0; \
meow_u64 a; \
for(a=0;a<4;a++){ \
xmm0=meow_aesenc(xmm0,meow_load(ptr+0x10+a*0x20),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr+0x00+a*0x20),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr+0x00+a*0x20),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr+0x10+a*0x20)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
} \
}
#else
#define MEOW_MIX2S(ptr) \
{ \
meow_u128 ttt0; \
ttt0=meow_aesenc(xmm0,meow_load(ptr+0x10),xmm4); \
xmm0=meow_aesenc(xmm4,meow_load(ptr+0x00),xmm8); \
xmm4=meow_aesenc(xmm8,meow_load(ptr+0x00),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr+0x10)); \
xmm8=meow_aesenc(xmm1,meow_load(ptr+0x30),xmm5); \
xmm1=meow_aesenc(xmm5,meow_load(ptr+0x20),xmm9); \
xmm5=meow_aesenc(xmm9,meow_load(ptr+0x20),xmm2); \
ttt0=meow_add(ttt0,meow_load(ptr+0x30)); \
xmm9=meow_aesenc(xmm2,meow_load(ptr+0x50),xmm6); \
xmm2=meow_aesenc(xmm6,meow_load(ptr+0x40),xmm10); \
xmm6=meow_aesenc(xmm10,meow_load(ptr+0x40),xmm3); \
xmm8=meow_add(xmm8,meow_load(ptr+0x50)); \
xmm10=meow_aesenc(xmm3,meow_load(ptr+0x70),xmm7); \
xmm3=meow_aesenc(xmm7,meow_load(ptr+0x60),ttt0); \
xmm7=meow_aesenc(ttt0,meow_load(ptr+0x60),xmm0); \
xmm9=meow_add(xmm9,meow_load(ptr+0x70)); \
}
#endif

#ifdef MEOW_DEF
#define MEOW_SHUFFLE2() \
{ \
meow_u128 ttt0; \
meow_u64 a; \
for(a=0;a<4;a++){ \
xmm0=meow_aesenc(xmm0,xmm8,xmm4); \
xmm4=meow_aesenc(xmm4,xmm1,xmm8); \
xmm8=meow_aesenc(xmm8,xmm10,xmm2); \
xmm9=meow_add(xmm9,xmm10); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
} \
}
#else
#define MEOW_SHUFFLE2() \
{ \
meow_u128 ttt0; \
ttt0=meow_aesenc(xmm0,xmm8,xmm4); \
xmm0=meow_aesenc(xmm4,xmm1,xmm8); \
xmm4=meow_aesenc(xmm8,xmm10,xmm2); \
xmm9=meow_add(xmm9,xmm10); \
xmm8=meow_aesenc(xmm1,xmm9,xmm5); \
xmm1=meow_aesenc(xmm5,xmm2,xmm9); \
xmm5=meow_aesenc(xmm9,ttt0,xmm3); \
xmm10=meow_add(xmm10,ttt0); \
xmm9=meow_aesenc(xmm2,xmm10,xmm6); \
xmm2=meow_aesenc(xmm6,xmm3,xmm10); \
xmm6=meow_aesenc(xmm10,xmm8,xmm0); \
ttt0=meow_add(ttt0,xmm8); \
xmm10=meow_aesenc(xmm3,ttt0,xmm7); \
xmm3=meow_aesenc(xmm7,xmm0,ttt0); \
xmm7=meow_aesenc(ttt0,xmm9,xmm1); \
xmm8=meow_add(xmm8,xmm9); \
}
#endif

#ifdef MEOW_DEF
#define MEOW_MIX41(ptr0,ptr1,ptr2,ptr3) \
{ \
meow_u128 ttt0; \
xmm0=meow_aesenc(xmm0,meow_load(ptr0+0x10),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr0+0x00),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr0+0x00),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr0+0x10)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
xmm0=meow_aesenc(xmm0,meow_load(ptr1+0x00),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr1+0x10),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr1+0x10),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr1+0x00)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
xmm0=meow_aesenc(xmm0,meow_load(ptr2+0x00),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr2+0x10),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr2+0x10),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr2+0x00)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
xmm0=meow_aesenc(xmm0,meow_load(ptr3+0x00),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr3+0x10),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr3+0x10),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr3+0x00)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
}
#else
#define MEOW_MIX41(ptr0,ptr1,ptr2,ptr3) \
{ \
meow_u128 ttt0; \
ttt0=meow_aesenc(xmm0,meow_load(ptr0+0x10),xmm4); \
xmm0=meow_aesenc(xmm4,meow_load(ptr0+0x00),xmm8); \
xmm4=meow_aesenc(xmm8,meow_load(ptr0+0x00),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr0+0x10)); \
xmm8=meow_aesenc(xmm1,meow_load(ptr1+0x00),xmm5); \
xmm1=meow_aesenc(xmm5,meow_load(ptr1+0x10),xmm9); \
xmm5=meow_aesenc(xmm9,meow_load(ptr1+0x10),xmm2); \
ttt0=meow_add(ttt0,meow_load(ptr1+0x00)); \
xmm9=meow_aesenc(xmm2,meow_load(ptr2+0x00),xmm6); \
xmm2=meow_aesenc(xmm6,meow_load(ptr2+0x10),xmm10); \
xmm6=meow_aesenc(xmm10,meow_load(ptr2+0x10),xmm3); \
xmm8=meow_add(xmm8,meow_load(ptr2+0x00)); \
xmm10=meow_aesenc(xmm3,meow_load(ptr3+0x00),xmm7); \
xmm3=meow_aesenc(xmm7,meow_load(ptr3+0x10),ttt0); \
xmm7=meow_aesenc(ttt0,meow_load(ptr3+0x10),xmm0); \
xmm9=meow_add(xmm9,meow_load(ptr3+0x00)); \
}
#endif

#ifdef MEOW_DEF
#define MEOW_MIX42(ptr0,ptr1,ptr2,ptr3) \
{ \
meow_u128 ttt0; \
xmm0=meow_aesenc(xmm0,meow_load(ptr0+0x00),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr0+0x10),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr0+0x10),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr0+0x00)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
xmm0=meow_aesenc(xmm0,meow_load(ptr1+0x00),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr1+0x10),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr1+0x10),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr1+0x00)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
xmm0=meow_aesenc(xmm0,meow_load(ptr2+0x10),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr2+0x00),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr2+0x00),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr2+0x10)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
xmm0=meow_aesenc(xmm0,meow_load(ptr3+0x10),xmm4); \
xmm4=meow_aesenc(xmm4,meow_load(ptr3+0x00),xmm8); \
xmm8=meow_aesenc(xmm8,meow_load(ptr3+0x00),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr3+0x10)); \
ttt0=xmm0; \
xmm0=xmm1; \
xmm1=xmm2; \
xmm2=xmm3; \
xmm3=xmm4; \
xmm4=xmm5; \
xmm5=xmm6; \
xmm6=xmm7; \
xmm7=xmm8; \
xmm8=xmm9; \
xmm9=xmm10; \
xmm10=ttt0; \
}
#else
#define MEOW_MIX42(ptr0,ptr1,ptr2,ptr3) \
{ \
meow_u128 ttt0; \
ttt0=meow_aesenc(xmm0,meow_load(ptr0+0x00),xmm4); \
xmm0=meow_aesenc(xmm4,meow_load(ptr0+0x10),xmm8); \
xmm4=meow_aesenc(xmm8,meow_load(ptr0+0x10),xmm1); \
xmm10=meow_add(xmm10,meow_load(ptr0+0x00)); \
xmm8=meow_aesenc(xmm1,meow_load(ptr1+0x00),xmm5); \
xmm1=meow_aesenc(xmm5,meow_load(ptr1+0x10),xmm9); \
xmm5=meow_aesenc(xmm9,meow_load(ptr1+0x10),xmm2); \
ttt0=meow_add(ttt0,meow_load(ptr1+0x00)); \
xmm9=meow_aesenc(xmm2,meow_load(ptr2+0x10),xmm6); \
xmm2=meow_aesenc(xmm6,meow_load(ptr2+0x00),xmm10); \
xmm6=meow_aesenc(xmm10,meow_load(ptr2+0x00),xmm3); \
xmm8=meow_add(xmm8,meow_load(ptr2+0x10)); \
xmm10=meow_aesenc(xmm3,meow_load(ptr3+0x10),xmm7); \
xmm3=meow_aesenc(xmm7,meow_load(ptr3+0x00),ttt0); \
xmm7=meow_aesenc(ttt0,meow_load(ptr3+0x00),xmm0); \
xmm9=meow_add(xmm9,meow_load(ptr3+0x10)); \
}
#endif

// NOTE(casey): The default seed is now a "nothing-up-our-sleeves" number for good measure.  You may verify that it is just an encoding of Pi.
/*
static meow_u8 MeowDefaultSeed[128] =
{
    0x32, 0x43, 0xF6, 0xA8, 0x88, 0x5A, 0x30, 0x8D,
    0x31, 0x31, 0x98, 0xA2, 0xE0, 0x37, 0x07, 0x34,
    0x4A, 0x40, 0x93, 0x82, 0x22, 0x99, 0xF3, 0x1D,
    0x00, 0x82, 0xEF, 0xA9, 0x8E, 0xC4, 0xE6, 0xC8,
    0x94, 0x52, 0x82, 0x1E, 0x63, 0x8D, 0x01, 0x37,
    0x7B, 0xE5, 0x46, 0x6C, 0xF3, 0x4E, 0x90, 0xC6,
    0xCC, 0x0A, 0xC2, 0x9B, 0x7C, 0x97, 0xC5, 0x0D,
    0xD3, 0xF8, 0x4D, 0x5B, 0x5B, 0x54, 0x70, 0x91,
    0x79, 0x21, 0x6D, 0x5D, 0x98, 0x97, 0x9F, 0xB1,
    0xBD, 0x13, 0x10, 0xBA, 0x69, 0x8D, 0xFB, 0x5A,
    0xC2, 0xFF, 0xD7, 0x2D, 0xBD, 0x01, 0xAD, 0xFB,
    0x7B, 0x8E, 0x1A, 0xFE, 0xD6, 0xA2, 0x67, 0xE9,
    0x6B, 0xA7, 0xC9, 0x04, 0x5F, 0x12, 0xC7, 0xF9,
    0x92, 0x4A, 0x19, 0x94, 0x7B, 0x39, 0x16, 0xCF,
    0x70, 0x80, 0x1F, 0x2E, 0x28, 0x58, 0xEF, 0xC1,
    0x66, 0x36, 0x92, 0x0D, 0x87, 0x15, 0x74, 0xE6
};
*/
meow_msc_align static meow_u8 MeowDefaultSeed[176] meow_gcc_align =
{
    0x32, 0x43, 0xF6, 0xA8, 0x88, 0x5A, 0x30, 0x8D,
    0x31, 0x31, 0x98, 0xA2, 0xE0, 0x37, 0x07, 0x34,
    0x4A, 0x40, 0x93, 0x82, 0x22, 0x99, 0xF3, 0x1D,
    0x00, 0x82, 0xEF, 0xA9, 0x8E, 0xC4, 0xE6, 0xC8,
    0x94, 0x52, 0x82, 0x1E, 0x63, 0x8D, 0x01, 0x37,
    0x7B, 0xE5, 0x46, 0x6C, 0xF3, 0x4E, 0x90, 0xC6,
    0xCC, 0x0A, 0xC2, 0x9B, 0x7C, 0x97, 0xC5, 0x0D,
    0xD3, 0xF8, 0x4D, 0x5B, 0x5B, 0x54, 0x70, 0x91,
    0x79, 0x21, 0x6D, 0x5D, 0x98, 0x97, 0x9F, 0xB1,
    0xBD, 0x13, 0x10, 0xBA, 0x69, 0x8D, 0xFB, 0x5A,
    0xC2, 0xFF, 0xD7, 0x2D, 0xBD, 0x01, 0xAD, 0xFB,
    0x7B, 0x8E, 0x1A, 0xFE, 0xD6, 0xA2, 0x67, 0xE9,
    0x6B, 0xA7, 0xC9, 0x04, 0x5F, 0x12, 0xC7, 0xF9,
    0x92, 0x4A, 0x19, 0x94, 0x7B, 0x39, 0x16, 0xCF,
    0x70, 0x80, 0x1F, 0x2E, 0x28, 0x58, 0xEF, 0xC1,
    0x66, 0x36, 0x92, 0x0D, 0x87, 0x15, 0x74, 0xE6,
    0x9A, 0x45, 0x8F, 0xEA, 0x3F, 0x49, 0x33, 0xD7,
    0xE0, 0xD9, 0x57, 0x48, 0xF7, 0x28, 0xEB, 0x65,
    0x87, 0x18, 0xBC, 0xD5, 0x88, 0x21, 0x54, 0xAE,
    0xE7, 0xB5, 0x4A, 0x41, 0xDC, 0x25, 0xA5, 0x9B,
    0x59, 0xC3, 0x0D, 0x53, 0x92, 0xAF, 0x26, 0x01,
    0x3C, 0x5D, 0x1B, 0x02, 0x32, 0x86, 0x08, 0x5F
};

/** Meow 1 **/

typedef struct meow1_state
{
    meow_u128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
    meow_u64 TotalLengthInBytes;
    
    int unsigned BufferLen;
    
    meow_msc_align meow_u8 Buffer[128] meow_gcc_align;
} meow1_state;

static void
Meow1Begin(meow1_state *State)
{
    State->xmm0=((meow_u128*)MeowDefaultSeed)[0];
    State->xmm1=((meow_u128*)MeowDefaultSeed)[1];
    State->xmm2=((meow_u128*)MeowDefaultSeed)[2];
    State->xmm3=((meow_u128*)MeowDefaultSeed)[3];
    State->xmm4=((meow_u128*)MeowDefaultSeed)[4];
    State->xmm5=((meow_u128*)MeowDefaultSeed)[5];
    State->xmm6=((meow_u128*)MeowDefaultSeed)[6];
    State->xmm7=((meow_u128*)MeowDefaultSeed)[7];
    
    State->BufferLen = 0;
    State->TotalLengthInBytes = 0;
}

static void
Meow1AbsorbBlocks(meow1_state *State, meow_u64 BlockCount, meow_u8 *rax)
{
    meow_u128 xmm0 = State->xmm0;
    meow_u128 xmm1 = State->xmm1;
    meow_u128 xmm2 = State->xmm2;
    meow_u128 xmm3 = State->xmm3;
    meow_u128 xmm4 = State->xmm4;
    meow_u128 xmm5 = State->xmm5;
    meow_u128 xmm6 = State->xmm6;
    meow_u128 xmm7 = State->xmm7;
    meow_u128 ttt0, ttt1, ttt2, ttt3;
    meow_u64 a;

    while(BlockCount--)
    {
        MEOW_MIX1(rax);
        
        rax += 0x80;
    }
    
    State->xmm0 = xmm0;
    State->xmm1 = xmm1;
    State->xmm2 = xmm2;
    State->xmm3 = xmm3;
    State->xmm4 = xmm4;
    State->xmm5 = xmm5;
    State->xmm6 = xmm6;
    State->xmm7 = xmm7;
}

static void
Meow1Absorb(meow1_state *State, meow_u64 Len, void *SourceInit)
{
    State->TotalLengthInBytes += Len;
    meow_u8 *Source = (meow_u8 *)SourceInit;
    
    // NOTE(casey): Handle any buffered residual
    if(State->BufferLen)
    {
        int unsigned Fill = (sizeof(State->Buffer) - State->BufferLen);
        if(Fill > Len)
        {
            Fill = (int unsigned)Len;
        }
        
        Len -= Fill;
        while(Fill--)
        {
            State->Buffer[State->BufferLen++] = *Source++;
        }
        
        if(State->BufferLen == sizeof(State->Buffer))
        {
            Meow1AbsorbBlocks(State, 1, State->Buffer);
            State->BufferLen = 0;
        }
    }
    
    // NOTE(casey): Handle any full blocks
    meow_u64 BlockCount = (Len >> 7);
    meow_u64 Advance = (BlockCount << 7);
    Meow1AbsorbBlocks(State, BlockCount, Source);
    
    Len -= Advance;
    Source += Advance;
    
    // NOTE(casey): Store residual
    while(Len--)
    {
        State->Buffer[State->BufferLen++] = *Source++;
    }
}

static meow_u128
Meow1End(meow1_state *State)
{
    meow_u64 a;
    if(State->BufferLen+16>sizeof(State->Buffer)){
        for(a=State->BufferLen;a<sizeof(State->Buffer);a++){
            State->Buffer[a]=0;
        }
        Meow1AbsorbBlocks(State,1,State->Buffer);
        State->BufferLen=0;
    }
    for(a=State->BufferLen;a<sizeof(State->Buffer);a++){
        State->Buffer[a]=0;
    }
    ((meow_u64*)(State->Buffer+(sizeof(State->Buffer)-16)))[0]=State->TotalLengthInBytes;
    Meow1AbsorbBlocks(State,1,State->Buffer);
    
    meow_u128 xmm0 = State->xmm0;
    meow_u128 xmm1 = State->xmm1;
    meow_u128 xmm2 = State->xmm2;
    meow_u128 xmm3 = State->xmm3;
    meow_u128 xmm4 = State->xmm4;
    meow_u128 xmm5 = State->xmm5;
    meow_u128 xmm6 = State->xmm6;
    meow_u128 xmm7 = State->xmm7;

    MEOW_SHUFFLE1();
    MEOW_SHUFFLE1();
    
    xmm0=meow_add(xmm0, xmm2);
    xmm1=meow_add(xmm1, xmm3);
    xmm4=meow_add(xmm4, xmm6);
    xmm5=meow_add(xmm5, xmm7);
    xmm0=meow_xor(xmm0, xmm1);
    xmm4=meow_xor(xmm4, xmm5);
    xmm0=meow_add(xmm0, xmm4);

    return(xmm0);
}

static meow_u128
Meow1Hash(meow_u64 Len, void *SourceInit)
{
    meow1_state State;
    Meow1Begin(&State);
    Meow1Absorb(&State,Len,SourceInit);
    return Meow1End(&State);
}

/** Meow 2 **/

typedef struct meow2_state
{
    meow_u128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10;
    meow_u64 TotalLengthInBytes;
    
    int unsigned BufferLen;
    
    meow_msc_align meow_u8 Buffer[128] meow_gcc_align;
} meow2_state;

static void
Meow2Begin(meow2_state *State, meow_u8 *Seed176)
{
    if(Seed176 == (meow_u8 *)0){
        Seed176 = MeowDefaultSeed;
    }
    
    State->xmm0=meow_load(Seed176+0x00);
    State->xmm1=meow_load(Seed176+0x10);
    State->xmm2=meow_load(Seed176+0x20);
    State->xmm3=meow_load(Seed176+0x30);
    State->xmm4=meow_load(Seed176+0x40);
    State->xmm5=meow_load(Seed176+0x50);
    State->xmm6=meow_load(Seed176+0x60);
    State->xmm7=meow_load(Seed176+0x70);
    State->xmm8=meow_load(Seed176+0x80);
    State->xmm9=meow_load(Seed176+0x90);
    State->xmm10=meow_load(Seed176+0xa0);
    
    State->BufferLen = 0;
    State->TotalLengthInBytes = 0;
}

static void
Meow2AbsorbBlocks(meow2_state *State, meow_u64 BlockCount, meow_u8 *rax)
{
    meow_u128 xmm0 = State->xmm0;
    meow_u128 xmm1 = State->xmm1;
    meow_u128 xmm2 = State->xmm2;
    meow_u128 xmm3 = State->xmm3;
    meow_u128 xmm4 = State->xmm4;
    meow_u128 xmm5 = State->xmm5;
    meow_u128 xmm6 = State->xmm6;
    meow_u128 xmm7 = State->xmm7;
    meow_u128 xmm8 = State->xmm8;
    meow_u128 xmm9 = State->xmm9;
    meow_u128 xmm10 = State->xmm10;

    while(BlockCount--)
    {
        MEOW_MIX2(rax)
        
        rax += 0x80;
    }
    
    State->xmm0 = xmm0;
    State->xmm1 = xmm1;
    State->xmm2 = xmm2;
    State->xmm3 = xmm3;
    State->xmm4 = xmm4;
    State->xmm5 = xmm5;
    State->xmm6 = xmm6;
    State->xmm7 = xmm7;
    State->xmm8 = xmm8;
    State->xmm9 = xmm9;
    State->xmm10 = xmm10;
}

static void
Meow2Absorb(meow2_state *State, meow_u64 Len, void *SourceInit)
{
    State->TotalLengthInBytes += Len;
    meow_u8 *Source = (meow_u8 *)SourceInit;
    
    // NOTE(casey): Handle any buffered residual
    if(State->BufferLen)
    {
        int unsigned Fill = (sizeof(State->Buffer) - State->BufferLen);
        if(Fill > Len)
        {
            Fill = (int unsigned)Len;
        }
        
        Len -= Fill;
        while(Fill--)
        {
            State->Buffer[State->BufferLen++] = *Source++;
        }
        
        if(State->BufferLen == sizeof(State->Buffer))
        {
            Meow2AbsorbBlocks(State, 1, State->Buffer);
            State->BufferLen = 0;
        }
    }
    
    // NOTE(casey): Handle any full blocks
    meow_u64 BlockCount = (Len >> 7);
    meow_u64 Advance = (BlockCount << 7);
    Meow2AbsorbBlocks(State, BlockCount, Source);
    
    Len -= Advance;
    Source += Advance;
    
    // NOTE(casey): Store residual
    while(Len--)
    {
        State->Buffer[State->BufferLen++] = *Source++;
    }
}

static meow_u128
Meow2End(meow2_state *State)
{
    meow_u64 a;
    if(State->BufferLen+16>sizeof(State->Buffer)){
        for(a=State->BufferLen;a<sizeof(State->Buffer);a++){
            State->Buffer[a]=0;
        }
        Meow2AbsorbBlocks(State,1,State->Buffer);
        State->BufferLen=0;
    }
    for(a=State->BufferLen;a<sizeof(State->Buffer);a++){
        State->Buffer[a]=0;
    }
    ((meow_u64*)(State->Buffer+(sizeof(State->Buffer)-16)))[0]=State->TotalLengthInBytes;
    Meow2AbsorbBlocks(State,1,State->Buffer);
    
    meow_u128 xmm0 = State->xmm0;
    meow_u128 xmm1 = State->xmm1;
    meow_u128 xmm2 = State->xmm2;
    meow_u128 xmm3 = State->xmm3;
    meow_u128 xmm4 = State->xmm4;
    meow_u128 xmm5 = State->xmm5;
    meow_u128 xmm6 = State->xmm6;
    meow_u128 xmm7 = State->xmm7;
    meow_u128 xmm8 = State->xmm8;
    meow_u128 xmm9 = State->xmm9;
    meow_u128 xmm10 = State->xmm10;

    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    
    State->xmm0 = xmm0;
    State->xmm1 = xmm1;
    State->xmm2 = xmm2;
    State->xmm3 = xmm3;
    State->xmm4 = xmm4;
    State->xmm5 = xmm5;
    State->xmm6 = xmm6;
    State->xmm7 = xmm7;
    State->xmm8 = xmm8;
    State->xmm9 = xmm9;
    State->xmm10 = xmm10;
    
    xmm0=meow_add(xmm0, xmm1);
    xmm2=meow_add(xmm2, xmm3);
    xmm4=meow_add(xmm4, xmm5);
    xmm6=meow_add(xmm6, xmm8);
    xmm7=meow_add(xmm7, xmm10);
    xmm0=meow_xor(xmm0, xmm6);
    xmm2=meow_xor(xmm2, xmm9);
    xmm4=meow_xor(xmm4, xmm7);
    xmm0=meow_add(xmm0, xmm2);
    xmm0=meow_add(xmm0, xmm4);

    return(xmm0);
}

static meow_u128
Meow2Hash(meow_u64 Len, void *SourceInit, meow_u8 *Seed176)
{
    meow2_state State;
    Meow2Begin(&State,Seed176);
    Meow2Absorb(&State,Len,SourceInit);
    return Meow2End(&State);
}

/** Meow Seed **/

static void
MeowExpandSeed(meow_u64 InputLen, void *Input, meow_u64 SeedLen, meow_u8 *SeedResult){
    meow2_state State;
    Meow2Begin(&State,(meow_u8 *)0);
    Meow2Absorb(&State,InputLen,Input);
    Meow2End(&State);
    
    meow_u128 xmm0 = State.xmm0;
    meow_u128 xmm1 = State.xmm1;
    meow_u128 xmm2 = State.xmm2;
    meow_u128 xmm3 = State.xmm3;
    meow_u128 xmm4 = State.xmm4;
    meow_u128 xmm5 = State.xmm5;
    meow_u128 xmm6 = State.xmm6;
    meow_u128 xmm7 = State.xmm7;
    meow_u128 xmm8 = State.xmm8;
    meow_u128 xmm9 = State.xmm9;
    meow_u128 xmm10 = State.xmm10;
    
    while(SeedLen>=32){
        meow_store(SeedResult,meow_xor(meow_add(xmm0,xmm1),meow_add(xmm2,xmm3)));
        meow_store(SeedResult+16,meow_xor(meow_add(xmm4,xmm5),meow_add(xmm6,xmm7)));
        //((meow_u128*)SeedResult)[0]=meow_xor(meow_add(xmm0,xmm1),meow_add(xmm2,xmm3));
        //((meow_u128*)SeedResult)[1]=meow_xor(meow_add(xmm4,xmm5),meow_add(xmm6,xmm7));
        MEOW_SHUFFLE2();
        SeedLen-=32;
        SeedResult+=32;
    }
    meow_u8 buffer[16];
    ((meow_u128*)buffer)[0]=meow_xor(meow_add(xmm0,xmm1),meow_add(xmm2,xmm3));
    if(SeedLen>=16){
        meow_store(SeedResult,((meow_u128*)buffer)[0]);
        //((meow_u128*)SeedResult)[0]=((meow_u128*)buffer)[0];
        ((meow_u128*)buffer)[0]=meow_xor(meow_add(xmm4,xmm5),meow_add(xmm6,xmm7));
        SeedLen-=16;
        SeedResult+=16;
    }
    int a;
    for(a=0;a<SeedLen;a++){
        SeedResult[a]=buffer[a];
    }
}

/** Meow 3 **/

typedef struct meow3_state
{
    meow_u128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10;
    meow_u64 TotalLengthInBytes;
    
    int unsigned BufferLen;
    
    meow_msc_align meow_u8 Buffer[512] meow_gcc_align;
} meow3_state;

static void
Meow3AbsorbBlocks(meow3_state *State, meow_u64 BlockCount, meow_u8 *rax)
{
    meow_u128 xmm0 = State->xmm0;
    meow_u128 xmm1 = State->xmm1;
    meow_u128 xmm2 = State->xmm2;
    meow_u128 xmm3 = State->xmm3;
    meow_u128 xmm4 = State->xmm4;
    meow_u128 xmm5 = State->xmm5;
    meow_u128 xmm6 = State->xmm6;
    meow_u128 xmm7 = State->xmm7;
    meow_u128 xmm8 = State->xmm8;
    meow_u128 xmm9 = State->xmm9;
    meow_u128 xmm10 = State->xmm10;

    while(BlockCount--)
    {
        MEOW_MIX2(rax);
        MEOW_MIX2(rax+0x80);
        MEOW_MIX2(rax+0x100);
        MEOW_MIX2(rax+0x180);
        MEOW_MIX2S(rax);
        xmm6=meow_add(xmm6,*(meow_u128*)MeowDefaultSeed);
        MEOW_MIX2S(rax+0x80);
        MEOW_MIX2S(rax+0x100);
        xmm2=meow_add(xmm2,*(meow_u128*)MeowDefaultSeed);
        MEOW_MIX2S(rax+0x180);
        rax += 0x200;
    }
    
    State->xmm0 = xmm0;
    State->xmm1 = xmm1;
    State->xmm2 = xmm2;
    State->xmm3 = xmm3;
    State->xmm4 = xmm4;
    State->xmm5 = xmm5;
    State->xmm6 = xmm6;
    State->xmm7 = xmm7;
    State->xmm8 = xmm8;
    State->xmm9 = xmm9;
    State->xmm10 = xmm10;
}

static void
Meow3Absorb(meow3_state *State, meow_u64 Len, void *SourceInit)
{
    State->TotalLengthInBytes += Len;
    meow_u8 *Source = (meow_u8 *)SourceInit;
    
    // NOTE(casey): Handle any buffered residual
    if(State->BufferLen)
    {
        int unsigned Fill = (sizeof(State->Buffer) - State->BufferLen);
        if(Fill > Len)
        {
            Fill = (int unsigned)Len;
        }
        
        Len -= Fill;
        while(Fill--)
        {
            State->Buffer[State->BufferLen++] = *Source++;
        }
        
        if(State->BufferLen == sizeof(State->Buffer))
        {
            Meow3AbsorbBlocks(State, 1, State->Buffer);
            State->BufferLen = 0;
        }
    }
    
    // NOTE(casey): Handle any full blocks
    meow_u64 BlockCount = (Len >> 9);
    meow_u64 Advance = (BlockCount << 9);
    Meow3AbsorbBlocks(State, BlockCount, Source);
    
    Len -= Advance;
    Source += Advance;
    
    // NOTE(casey): Store residual
    while(Len--)
    {
        State->Buffer[State->BufferLen++] = *Source++;
    }
}

static void
Meow3Begin(meow3_state *State, meow_u8 *Seed176)
{
    State->xmm0=meow_load(Seed176+0x00);
    State->xmm1=meow_load(Seed176+0x10);
    State->xmm2=meow_load(Seed176+0x20);
    State->xmm3=meow_load(Seed176+0x30);
    State->xmm4=meow_load(Seed176+0x40);
    State->xmm5=meow_load(Seed176+0x50);
    State->xmm6=meow_load(Seed176+0x60);
    State->xmm7=meow_load(Seed176+0x70);
    State->xmm8=meow_load(Seed176+0x80);
    State->xmm9=meow_load(Seed176+0x90);
    State->xmm10=meow_load(Seed176+0xa0);
    
    State->BufferLen = 0;
    State->TotalLengthInBytes = 0;
}
/*
static void
Meow3Begin(meow3_state *State, meow_u8 *Seed32)
{
    State->xmm0=meow_add(((meow_u128*)MeowDefaultSeed)[0],((meow_u128*)Seed32)[0]);
    State->xmm1=meow_add(((meow_u128*)MeowDefaultSeed)[1],((meow_u128*)Seed32)[0]);
    State->xmm2=meow_add(((meow_u128*)MeowDefaultSeed)[2],((meow_u128*)Seed32)[1]);
    State->xmm3=meow_add(((meow_u128*)MeowDefaultSeed)[3],((meow_u128*)Seed32)[1]);
    State->xmm4=meow_sub(((meow_u128*)MeowDefaultSeed)[4],((meow_u128*)Seed32)[0]);
    State->xmm5=meow_sub(((meow_u128*)MeowDefaultSeed)[5],((meow_u128*)Seed32)[0]);
    State->xmm6=meow_sub(((meow_u128*)MeowDefaultSeed)[6],((meow_u128*)Seed32)[1]);
    State->xmm7=meow_sub(((meow_u128*)MeowDefaultSeed)[7],((meow_u128*)Seed32)[1]);
    State->xmm8=meow_xor(((meow_u128*)MeowDefaultSeed)[8],((meow_u128*)Seed32)[0]);
    State->xmm9=meow_xor(((meow_u128*)MeowDefaultSeed)[9],((meow_u128*)Seed32)[0]);
    State->xmm10=meow_xor(((meow_u128*)MeowDefaultSeed)[10],((meow_u128*)Seed32)[1]);
    
    State->BufferLen = 0;
    State->TotalLengthInBytes = 0;
}
*/

static meow_u128
Meow3End(meow3_state *State)
{
    meow_u64 a;
    if(State->BufferLen+16>sizeof(State->Buffer)){
        for(a=State->BufferLen;a<sizeof(State->Buffer);a++){
            State->Buffer[a]=0;
        }
        Meow3AbsorbBlocks(State,1,State->Buffer);
        State->BufferLen=0;
    }
    for(a=State->BufferLen;a<sizeof(State->Buffer);a++){
        State->Buffer[a]=0;
    }
    ((meow_u64*)(State->Buffer+(sizeof(State->Buffer)-16)))[0]=State->TotalLengthInBytes;
    Meow3AbsorbBlocks(State,1,State->Buffer);
    
    meow_u128 xmm0 = State->xmm0;
    meow_u128 xmm1 = State->xmm1;
    meow_u128 xmm2 = State->xmm2;
    meow_u128 xmm3 = State->xmm3;
    meow_u128 xmm4 = State->xmm4;
    meow_u128 xmm5 = State->xmm5;
    meow_u128 xmm6 = State->xmm6;
    meow_u128 xmm7 = State->xmm7;
    meow_u128 xmm8 = State->xmm8;
    meow_u128 xmm9 = State->xmm9;
    meow_u128 xmm10 = State->xmm10;

    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    
    xmm0=meow_add(xmm0, xmm1);
    xmm2=meow_add(xmm2, xmm3);
    xmm4=meow_add(xmm4, xmm5);
    xmm6=meow_add(xmm6, xmm8);
    xmm7=meow_add(xmm7, xmm10);
    xmm0=meow_xor(xmm0, xmm6);
    xmm2=meow_xor(xmm2, xmm9);
    xmm4=meow_xor(xmm4, xmm7);
    xmm0=meow_add(xmm0, xmm2);
    xmm0=meow_add(xmm0, xmm4);

    return(xmm0);
}

static meow_u128
Meow3Hash(meow_u64 Len, void *SourceInit, meow_u8 *Seed176)
{
    meow3_state State;
    Meow3Begin(&State,Seed176);
    Meow3Absorb(&State,Len,SourceInit);
    return Meow3End(&State);
}

/** Meow 4 **/

typedef struct meow4_state
{
    meow_u128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10;
    meow_u64 TotalLengthInBytes;
    
    int unsigned BufferLen;
    
    meow_msc_align meow_u8 Buffer[1024] meow_gcc_align;
} meow4_state;

static void
Meow4AbsorbBlocks(meow4_state *State, meow_u64 BlockCount, meow_u8 *rax)
{
    meow_u128 xmm0 = State->xmm0;
    meow_u128 xmm1 = State->xmm1;
    meow_u128 xmm2 = State->xmm2;
    meow_u128 xmm3 = State->xmm3;
    meow_u128 xmm4 = State->xmm4;
    meow_u128 xmm5 = State->xmm5;
    meow_u128 xmm6 = State->xmm6;
    meow_u128 xmm7 = State->xmm7;
    meow_u128 xmm8 = State->xmm8;
    meow_u128 xmm9 = State->xmm9;
    meow_u128 xmm10 = State->xmm10;
    
    meow_u64 a;

    while(BlockCount--)
    {
        for(a=0;a<0x400;a+=0x80){
            MEOW_MIX41(rax+a+0x00,rax+a+0x20,rax+a+0x40,rax+a+0x60);
        }
        xmm6=meow_sub(xmm6,*(meow_u128*)MeowDefaultSeed);
        /*
        for(a=0;a<0x100;a+=0x20){
            MEOW_MIX42(rax+a+0x00,rax+a+0x100,rax+a+0x200,rax+a+0x300);
        }
        */
        for(a=0;a<0x1400;a+=0x280){
           MEOW_MIX42(rax+(a+0x00)%0x400,rax+(a+0xa0)%0x400,rax+(a+0x140)%0x400,rax+(a+0x1e0)%0x400);
        }
        xmm2=meow_sub(xmm2,*(meow_u128*)MeowDefaultSeed);
        /*
        for(a=0;a<0x80;a+=0x20){
            MEOW_MIX41(rax+a+0x00,rax+a+0x80,rax+a+0x100,rax+a+0x180);
        }
        xmm6=meow_add(xmm6,*(meow_u128*)MeowDefaultSeed);
        for(a=0x200;a<0x280;a+=0x20){
            MEOW_MIX41(rax+a+0x00,rax+a+0x80,rax+a+0x100,rax+a+0x180);
        }
        */
        for(a=0;a<0x1c00;a+=0x380){
           MEOW_MIX41(rax+(a+0x00)%0x400,rax+(a+0xe0)%0x400,rax+(a+0x1c0)%0x400,rax+(a+0x2a0)%0x400);
        }
        for(a=0;a<0x400;a+=0x100){
            MEOW_MIX42(rax+a+0x00,rax+a+0x40,rax+a+0x80,rax+a+0xc0);
            xmm2=meow_add(xmm2,*(meow_u128*)MeowDefaultSeed);
            MEOW_MIX42(rax+a+0x20,rax+a+0x60,rax+a+0xa0,rax+a+0xe0);
        }
        rax += 0x400;
    }
    
    State->xmm0 = xmm0;
    State->xmm1 = xmm1;
    State->xmm2 = xmm2;
    State->xmm3 = xmm3;
    State->xmm4 = xmm4;
    State->xmm5 = xmm5;
    State->xmm6 = xmm6;
    State->xmm7 = xmm7;
    State->xmm8 = xmm8;
    State->xmm9 = xmm9;
    State->xmm10 = xmm10;
}

static void
Meow4Absorb(meow4_state *State, meow_u64 Len, void *SourceInit)
{
    State->TotalLengthInBytes += Len;
    meow_u8 *Source = (meow_u8 *)SourceInit;
    
    // NOTE(casey): Handle any buffered residual
    if(State->BufferLen)
    {
        int unsigned Fill = (sizeof(State->Buffer) - State->BufferLen);
        if(Fill > Len)
        {
            Fill = (int unsigned)Len;
        }
        
        Len -= Fill;
        while(Fill--)
        {
            State->Buffer[State->BufferLen++] = *Source++;
        }
        
        if(State->BufferLen == sizeof(State->Buffer))
        {
            Meow4AbsorbBlocks(State, 1, State->Buffer);
            State->BufferLen = 0;
        }
    }
    
    // NOTE(casey): Handle any full blocks
    meow_u64 BlockCount = (Len >> 10);
    meow_u64 Advance = (BlockCount << 10);
    Meow4AbsorbBlocks(State, BlockCount, Source);
    
    Len -= Advance;
    Source += Advance;
    
    // NOTE(casey): Store residual
    while(Len--)
    {
        State->Buffer[State->BufferLen++] = *Source++;
    }
}

static void
Meow4Begin(meow4_state *State)
{
    State->xmm0=((meow_u128*)MeowDefaultSeed)[0];
    State->xmm1=((meow_u128*)MeowDefaultSeed)[1];
    State->xmm2=((meow_u128*)MeowDefaultSeed)[2];
    State->xmm3=((meow_u128*)MeowDefaultSeed)[3];
    State->xmm4=((meow_u128*)MeowDefaultSeed)[4];
    State->xmm5=((meow_u128*)MeowDefaultSeed)[5];
    State->xmm6=((meow_u128*)MeowDefaultSeed)[6];
    State->xmm7=((meow_u128*)MeowDefaultSeed)[7];
    State->xmm8=((meow_u128*)MeowDefaultSeed)[8];
    State->xmm9=((meow_u128*)MeowDefaultSeed)[9];
    State->xmm10=((meow_u128*)MeowDefaultSeed)[10];
    
    State->BufferLen = 0;
    State->TotalLengthInBytes = 0;
}

static void
Meow4End(meow4_state *State, void *Out64)
{
    meow_u64 a;
    if(State->BufferLen+16>sizeof(State->Buffer)){
        for(a=State->BufferLen;a<sizeof(State->Buffer);a++){
            State->Buffer[a]=0;
        }
        Meow4AbsorbBlocks(State,1,State->Buffer);
        State->BufferLen=0;
    }
    for(a=State->BufferLen;a<sizeof(State->Buffer);a++){
        State->Buffer[a]=0;
    }
    ((meow_u64*)(State->Buffer+(sizeof(State->Buffer)-16)))[0]=State->TotalLengthInBytes;
    Meow4AbsorbBlocks(State,1,State->Buffer);
    
    meow_u128 xmm0 = State->xmm0;
    meow_u128 xmm1 = State->xmm1;
    meow_u128 xmm2 = State->xmm2;
    meow_u128 xmm3 = State->xmm3;
    meow_u128 xmm4 = State->xmm4;
    meow_u128 xmm5 = State->xmm5;
    meow_u128 xmm6 = State->xmm6;
    meow_u128 xmm7 = State->xmm7;
    meow_u128 xmm8 = State->xmm8;
    meow_u128 xmm9 = State->xmm9;
    meow_u128 xmm10 = State->xmm10;

    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    MEOW_SHUFFLE2();
    
    meow_store(Out64,meow_add(xmm0,xmm1));
    meow_store(Out64+16,meow_xor(meow_add(xmm2,xmm3),xmm8));
    meow_store(Out64+32,meow_xor(meow_add(xmm4,xmm5),xmm9));
    meow_store(Out64+48,meow_xor(meow_add(xmm6,xmm7),xmm10));
}

static void
Meow4Hash(meow_u64 Len, void *SourceInit, void *Out64)
{
    meow4_state State;
    Meow4Begin(&State);
    Meow4Absorb(&State,Len,SourceInit);
    Meow4End(&State,Out64);
}



#define MEOW_HASH_X64_AESNI_H
#endif
