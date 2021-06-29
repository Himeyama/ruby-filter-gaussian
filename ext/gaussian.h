/*
 *  (c) 2021 Murata Mitsuharu
 *  Licensed under the MIT License.
 *  source: https://github.com/Himeyama/C-Gaussian-Filter
 */


#ifndef GAUSSIAN_H
#define GAUSSIAN_H
 
struct vector{
    long size;
    double* data;
};

typedef struct vector Vector;

typedef struct{
    Vector src_data;
    Vector* dst_data;
    double sd;
    double truncate;
} GaussianArgsRet;

typedef struct{
    unsigned long src;
    unsigned long dst;
    unsigned long size;
    double sd;
    double truncate;
} GaussianArgsRetRb;

void Vector_p(Vector vec);
void Vector_txt(Vector vec);
void Vector_destroy(Vector vec);
Vector Vector_initialize(long size);
Vector Vector_zeros(long size);
double Vector_dot(Vector vec1, Vector vec2);
Vector Vector_arange(int start, int stop);
double Vector_sum(Vector vec);
void Vector_div(Vector vec, double n);
Vector Vector_normal(long size);
Vector Vector_clone(Vector vec);
void* gaussian(GaussianArgsRet* ga);
void* _gaussian_filter1d_dfloat();

#endif