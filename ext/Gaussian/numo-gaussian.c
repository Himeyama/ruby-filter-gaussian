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

// ガウシアンフィルターの重み配列生成
// sd: 標準偏差
// n: カーネルの大きさ (n * 2 + 1)
f64* gaussian_kernel(f64 sd, i64 n){
    f64* w = (f64*)malloc(sizeof(f64) * (2 * n + 1));
    f64 sum = 0;
    for(i64 i = -n; i <= n; i++)
        sum += (w[i+n] = exp(-0.5 * i * i / (sd * sd)));
    for(i64 i = -n; i <= n; i++)
        w[i+n] /= sum;
    return w;
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


// データの両端部分を補完
// v: 配列
// n: 配列の大きさ
// len: 片端補完サイズ
f64* dreflect(f64 *v, i64 n, i64 len){
    if(n < len){
        fprintf(stderr, "エラー: 両端の大きさが元の配列より大きいため、補完できません。\n");
        return NULL;
    };
    i64 r_size = n + 2 * len;
    f64* r = (f64*)malloc(sizeof(f64) * r_size);
    for(i64 i = 0; i < n; i++){
        r[len + i] = v[i];
    }
    for(i64 i = 0; i < len; i++){
        r[len - i - 1] = v[i];
        r[len + n + i] = v[n - i - 1];
    }
    return r;
}

f64* correlate1d(f64* input, i64 input_size, f64* weights, i64 weights_size, f64* dst){
    i64 n = input_size - weights_size;
    f64* r = dst;
    for(i64 i = 0; i <= n; i++){
        f64 sum = 0;
        for(i64 j = 0; j < weights_size; j++)
            sum += weights[weights_size - j - 1] * input[i + j];
        r[i] = sum;
    }
    return r;
}


// 反転したベクトルを返します
// v: 配列
// size: 配列の大きさ
f64* dreverse(f64 v[], i64 size){
    f64* r = malloc(sizeof(f64) * size);
    for(i64 i = 0; i < size; i++)
        r[i] = v[size-i-1];
    return r;
}


void* _gaussian_filter1d_dfloat(GaussianArgsRetRb* ga){
    f64* dst = ga->dst;
    f64* src = ga->src;
    i64 size = ga->size;
    dst += size * ga->i;
    src += size * ga->i;
    f64 sd = ga->sd;
    f64 truncate = ga->truncate;
    i64 t = (i64)(truncate * sd + 0.5);
    if(t > size){
        fprintf(stderr, "データが小さすぎます\n");
        exit(EXIT_FAILURE);
    }
    // i64 gs = 2 * t + 1;
    f64* tmp = gaussian_kernel(sd, t);
    f64* weights = dreverse(tmp, 2 * t + 1);
    f64* s = dreflect(src, size, t);
    correlate1d(s, 2 * t + size, weights, 2 * t + 1, dst);

    free(tmp);
    free(weights);
    free(s);

    return NULL;
}