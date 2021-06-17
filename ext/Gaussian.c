/*
 *  (c) 2021 Murata Mitsuharu
 *  Licensed under the MIT License.
 */

#include <stdio.h>
#include <ruby.h>
#include "gaussian.h"

VALUE cGaussian;

static VALUE gaussian_filter1d(VALUE self, VALUE ary){
    Vector vec, filtered;
    VALUE a;
    
    if(TYPE(ary) != T_ARRAY) return Qfalse;
    
    long size = RARRAY_LEN(ary);
    a = rb_ary_new_capa(size); 
    vec = Vector_initialize(size);
    for(long i = 0; i < size; i++){
        vec.data[i] = rb_ary_entry(ary, i); 
    }
    filtered = gaussian(vec, 1, 4.0);
    
    for(long i = 0; i < size; i++){
        rb_ary_store(a, i, DBL2NUM(filtered.data[i]));
    }

    Vector_destroy(vec);
    Vector_destroy(filtered);

    return a;
}

void Init_Gaussian(void){
    cGaussian = rb_define_class("Gaussian", rb_cObject);
    rb_define_singleton_method(cGaussian, "filter1d", gaussian_filter1d, 1);
}
