#ifndef NUMO_GAUSSIAN_H
#define NUMO_GAUSSIAN_H

typedef narray_t* NArray;
typedef size_t SIZE;
typedef __int64_t i64;
typedef __uint32_t u32;
typedef __uint64_t u64;
typedef double f64;

typedef struct{
    VALUE src_data;
    VALUE dst_data;
    f64 sd;
    f64 truncate;
} GaussianNArrayArgsRet;

f64 getData(f64* data, u64 idx);
f64 conv1(f64* data, f64 idx, u32 r);
void* narray_gaussian(GaussianNArrayArgsRet* ga);


#endif