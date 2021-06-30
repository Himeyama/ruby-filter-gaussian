#ifndef NUMO_GAUSSIAN_H
#define NUMO_GAUSSIAN_H

typedef narray_t* NArray;
typedef size_t SIZE;
typedef unsigned char u8;
typedef __int64_t i64;
typedef __uint32_t u32;
typedef __uint64_t u64;
typedef double f64;
typedef u64 VALUE;

typedef struct{
    VALUE src_data;
    VALUE dst_data;
    f64 sd;
    f64 truncate;
} GaussianNArrayArgsRet;

typedef struct{
    f64* src;
    f64* dst;
    VALUE size;
    f64 sd;
    f64 truncate;
    u64 i;
} GaussianArgsRetRb;

f64 getData(f64* data, u64 idx);
f64 conv1(f64* data, f64 idx, u32 r);
void* narray_gaussian(GaussianNArrayArgsRet* ga);
void* _gaussian_filter1d_dfloat(GaussianArgsRetRb* ga);

#endif