/*
 *  (c) 2021 Murata Mitsuharu
 *  Licensed under the MIT License.
 */

#include <ruby/ruby.h>
#include <numo/narray.h>
#include "numo-gaussian.h"

f64 getData(f64* data, u64 idx){
    return 0;
}

f64 conv1(f64* data, f64 idx, u32 r){
    for(i64 i = idx - r; i <= idx + r; i++){
        printf("%ld\n", i);
    }
    return 0;
}

// index 取得
i64 get_at(i64 i, i64 n){
    i64 idx;
    if(i >= -n && i < 2 * n){
        idx = i < 0 ? - i - 1 : (n <= i ? 2 * n - i - 1 : i);
        // printf("INDEX: %ld\n",idx);
        return idx;
    }
    return -1;
}

// 数値のポインタを返す
f64* get_mat_at(f64* x, i64 n, i64 i){
    i64 idx = get_at(i, n);
    if(idx == -1) return NULL;
    return x + idx;
}

f64 n_dot(i64 n, i64 r, i64 size, f64 *x, i64 i, f64 *gk){
    f64 sum = 0;
    f64* tmp;
    for(i64 j = 0; j < n; j++){
        tmp = get_mat_at(x, size, j+i-r);
        // printf("%f * %f\n", gk[j], *tmp);
        if(tmp == NULL) return NAN;
        sum += gk[j] * *tmp;
    }
    // puts("OK");
    return sum;
}


f64* n_gauss_func(i64 r, f64 sd){
    f64* g = (f64*)malloc(sizeof(f64)*(2*r+1));
    f64 sum = 0;
    for(i64 i = -r; i <= r; i++){
        g[i+r] = exp(-0.5 * i * i / (sd * sd));
        sum += g[i+r];
    }
    for(i64 i = -r; i <= r; i++)
        g[i+r] /= sum;
    return g;
}


void* _gaussian_filter1d_dfloat(GaussianArgsRetRb* ga){
    f64* dst = ga->dst;
    f64* src = ga->src;
    i64 size = ga->size;
    dst += size * ga->i;
    src += size * ga->i;
    f64 sd = ga->sd;
    f64 truncate = ga->truncate;
    i64 r = (i64)(truncate * sd + 0.5);
    if(r > size){
        fprintf(stderr, "データが小さすぎます\n");
        exit(EXIT_FAILURE);
    }
    i64 gs = 2*r+1;
    f64* gk = n_gauss_func(r, sd);
    for(i64 i = 0; i < size; i++)
        dst[i] = n_dot(gs, r, size, src, i, gk);
    free(gk);
    return NULL;
}