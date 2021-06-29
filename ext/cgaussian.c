/*
 *  (c) 2021 Murata Mitsuharu
 *  Licensed under the MIT License.
 *  source: https://github.com/Himeyama/C-Gaussian-Filter
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
// #include <pthread.h>
#include <ruby.h>
#include <numo/narray.h>
#include "gaussian.h"


void Vector_p(Vector vec){
    double* data = vec.data;
    char* str = (char*)malloc(100000000); // 100MB
    char d[16];
    for(int i = 0; i < vec.size; i++){
        sprintf(d, i == vec.size - 1 ? "%f" : "%f, ", data[i]);
        // ＼(^o^)／
        strcat(str, d);
    }
    printf("[%s]\n", str);
}

void Vector_txt(Vector vec){
    double* data = vec.data;
    char* str = (char*)malloc(100000000); // 100MB
    char d[16];
    for(int i = 0; i < vec.size; i++){
        sprintf(d, "%f\n", data[i]);
        // ＼(^o^)／
        strcat(str, d);
    }
    printf("%s", str);
}

void Vector_destroy(Vector vec){
    free(vec.data);
}

Vector Vector_initialize(long size){
    Vector vec;
    vec.size = size;
    vec.data = (double*)malloc(sizeof(double) * size);
    return vec;
}

Vector Vector_initializeP(long size, Vector* vec){
    vec->size = size;
    vec->data = (double*)malloc(sizeof(double) * size);
    return *vec;
}

Vector Vector_zeros(long size){
    Vector vec = Vector_initialize(size);
    for(int i = 0; i < size; i++)
        vec.data[i] = 0;
    return vec;
}

double Vector_dot(Vector vec1, Vector vec2){
    if(vec1.size != vec2.size){
        fprintf(stderr, "ベクトルのサイズが異なります\n");
        exit(EXIT_FAILURE);
    }
    double dot = 0;
    for(int i = 0; i < vec1.size; i++){
        dot += vec1.data[i] * vec2.data[i];
    }
    return dot;
}

Vector Vector_arange(int start, int stop){
    int size = stop - start;
    Vector vec = Vector_initialize(size);
    for(int i = 0; i < size; i++){
        vec.data[i] = start + i;
    }
    return vec;
}

double Vector_sum(Vector vec){
    double sum = 0;
    for(int i = 0; i < vec.size; i++)
        sum += vec.data[i];
    return sum;
}

// 破壊的関数
void Vector_div(Vector vec, double n){
    for(int i = 0; i < vec.size; i++){
        vec.data[i] /= n;
    }
}

// 正規分布 (0, 1) に従う乱数
Vector Vector_normal(long size){
    Vector vec = Vector_initialize(size);
    for(int i = 0; i < size; i++){
        double u1 = drand48();
        double u2 = drand48();
        vec.data[i] = sqrt(-2 * log(u1)) * sin(2 * M_PI * u2);
    }
    return vec;
}

Vector Vector_clone(Vector vec){
    Vector c = Vector_initialize(vec.size);
    for(int i = 0; i < vec.size; i++)
        c.data[i] = vec.data[i];
    return c;
}

void* gaussian(GaussianArgsRet* ga){
    Vector src_data = ga->src_data;
    double sd = ga->sd;
    double truncate = ga->truncate;
    
    // printf("sd: %lf\n", sd);
    // printf("truncate: %lf\n", truncate);
    // Vector_p(src_data);

    int r = (int)(truncate * sd + 0.5);
    
    if(r > src_data.size){
        fprintf(stderr, "データが小さすぎます\n");
        exit(EXIT_FAILURE);
    }
    
    Vector data = Vector_initialize(src_data.size + 2 * r);
    for(int i = 0; i < r; i++)
        data.data[i] = src_data.data[r-i-1];
    for(int i = r; i < src_data.size + r; i++)
        data.data[i] = src_data.data[i-r];
    for(int i = 0; i < r; i++)
        data.data[i+r+src_data.size] = src_data.data[src_data.size-i-1];

    Vector f = Vector_initialize(data.size);

    Vector x = Vector_arange(-r, r+1);
    Vector gauss = Vector_zeros(2*r+1);
    for(int i = 0; i < x.size; i++){
        gauss.data[i] = exp(-0.5 * x.data[i] * x.data[i] / (sd * sd));
    }
    Vector_div(gauss, Vector_sum(gauss));

    for(int i = r; i < data.size - r; i++){
        Vector d;
        d.size = 2 * r + 1;
        d.data = data.data + i - r;
        f.data[i] = Vector_dot(gauss, d);
    }

    Vector *filtered = ga->dst_data;
    Vector_initializeP(src_data.size, filtered);
    for(int i = 0; i < filtered->size; i++){
        filtered->data[i] = f.data[i + r];
    }

    Vector_destroy(data);
    Vector_destroy(f);
    Vector_destroy(x);
    Vector_destroy(gauss);

    return NULL;
}

ulong get_at(long i, ulong n){
    if(i >= -n && i < 2 * n)
        return i < 0 ? 0 : (n <= 1 ? n - i - 1 : i);
    return NAN;
}

double* get_mat_at(double* x, ulong n, long i){
    ulong idx = get_at(i, n);
    if(idx == NAN) return NULL;
    return x + idx;
}

double n_dot(ulong n, ulong size, double *x, ulong i, double *y){
    double sum = 0;
    for(int i = 0; i < n; i++)
        sum += y[i] * (*get_mat_at(x, size, i));
    return sum;
}


double* n_gauss_func(ulong r, double sd){
    double* g = (double*)malloc(sizeof(double)*(2*r+1));
    for(int i = -r; i <= r; i++){
        g[i+r] = exp(-0.5 * i * i / (sd * sd));
    }
    return g;
}


void* _gaussian_filter1d_dfloat(GaussianArgsRetRb* ga){
    double* dst = (double*)na_get_pointer_for_read(ga->dst);
    double* src = (double*)na_get_pointer_for_read(ga->src);
    ulong size =  ga->size; // データのサイズ
    double sd = ga->sd;
    double truncate = ga->truncate;
    int r = (int)(truncate * sd + 0.5);
    // printf("%lf\n", sd);
    // rb_p(r);
    if(r > size){
        fprintf(stderr, "データが小さすぎます\n");
        exit(EXIT_FAILURE);
    }
    double* g = n_gauss_func(r, sd); //ガウスカーネル
    
    

    for(int i = 0; i < size; i++){
        dst[i] = n_dot(2*r+1, size, src, i, g);
    }
    dst[0] = -100;

    // return NULL;

    free(g);
    return NULL;
}