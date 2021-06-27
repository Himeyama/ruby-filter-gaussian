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

void* narray_gaussian(GaussianNArrayArgsRet* ga){
    // struct NARRAY *src_data, *dst_data;
    // // VALUE src2 = na_cast_object(ga->src_data, 5);
    // rb_funcall(rb_cObject, rb_intern("p"), 1, DBL2NUM(1.2));
    // rb_funcall(rb_cObject, rb_intern("p"), 1, ga->src_data);
    // GetNArray(src2, src_data);
    // GetNArray(na_cast_object(ga->dst_data, 5), dst_data);
    // f64 sd = ga->sd;
    // f64 truncate = ga->truncate;
    // u32 r = (u32)(truncate * sd + 0.5);
    // SIZE size = NA_SHAPE(src_data)[0];

    // if(r > size){
    //     fprintf(stderr, "データが小さすぎます\n");
    //     return NULL;
    // }
    // f64* data = (f64*)na_get_pointer_for_read(ga->src_data);
    // // f64* data = (f64*)malloc(sizeof(f64) * (size + r * 2));
    // f64* f = (f64*)na_get_pointer_for_read(ga->dst_data);

    // for(u64 i = 0; i < size; i++){
    //     f[i] = conv1(data, i, r);
    // }


    return NULL;
}