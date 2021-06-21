/*
 *  (c) 2021 Murata Mitsuharu
 *  Licensed under the MIT License.
 */

#include <ruby/ruby.h>
#include <numo/narray.h>

typedef narray_t* NArray;
typedef size_t SIZE;
typedef __u32 u32;
typedef __u64 u64;
typedef double f64;

typedef struct{
    VALUE src_data;
    VALUE dst_data;
    f64 sd;
    f64 truncate;
} GaussianNArrayArgsRet;

void* narray_gaussian(GaussianNArrayArgsRet* ga){
    NArray src_data, dst_data;
    GetNArray(ga->src_data, src_data);
    GetNArray(ga->dst_data, dst_data);

    f64 sd = ga->sd;
    f64 truncate = ga->truncate;
    u32 r = (u32)(truncate * sd + 0.5);
    SIZE size = NA_SHAPE(src_data)[0];
    if(r > size){
        fprintf(stderr, "データが小さすぎます\n");
        return NULL;
    }
    f64* sdata = (f64*)na_get_pointer_for_read(ga->src_data);
    f64* data = (f64*)malloc(sizeof(f64) * (size + r * 2));

    for(u64 i = 0; i < r; i++)
        data[i] = sdata[r-i-1];
    for(u64 i = r; i < size + r; i++)
        data[i] = sdata[i-r];
    for(u64 i = 0; i < r; i++)
        data[i+r+size] = sdata[size-i-1];



    free(data);

    
    
    size;
    r;

    return NULL;
}
    
//     Vector data = Vector_initialize(src_data.size + 2 * r);
//     for(int i = 0; i < r; i++)
//         data.data[i] = src_data.data[r-i-1];
//     for(int i = r; i < src_data.size + r; i++)
//         data.data[i] = src_data.data[i-r];
//     for(int i = 0; i < r; i++)
//         data.data[i+r+src_data.size] = src_data.data[src_data.size-i-1];

//     Vector f = Vector_initialize(data.size);

//     Vector x = Vector_arange(-r, r+1);
//     Vector gauss = Vector_zeros(2*r+1);
//     for(int i = 0; i < x.size; i++){
//         gauss.data[i] = exp(-0.5 * x.data[i] * x.data[i] / (sd * sd));
//     }
//     Vector_div(gauss, Vector_sum(gauss));

//     for(int i = r; i < data.size - r; i++){
//         Vector d;
//         d.size = 2 * r + 1;
//         d.data = data.data + i - r;
//         f.data[i] = Vector_dot(gauss, d);
//     }

//     Vector *filtered = ga->dst_data;
//     Vector_initializeP(src_data.size, filtered);
//     for(int i = 0; i < filtered->size; i++){
//         filtered->data[i] = f.data[i + r];
//     }

//     Vector_destroy(data);
//     Vector_destroy(f);
//     Vector_destroy(x);
//     Vector_destroy(gauss);

//     return NULL;
// }