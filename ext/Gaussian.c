/*
 *  (c) 2021 Murata Mitsuharu
 *  Licensed under the MIT License.
 */

#include <stdio.h>
#include <ruby.h>
#include "gaussian.h"

VALUE cGaussian;

static VALUE gaussian_filter1d(VALUE self, VALUE ary, VALUE sd){
    // rb_funcall(rb_cObject, rb_intern("p"), 1, sd);

    Vector vec, filtered;
    VALUE a;
    
    if(TYPE(ary) != T_ARRAY) return Qfalse;
    
    long size = RARRAY_LEN(ary);
    a = rb_ary_new_capa(size); 
    vec = Vector_zeros(size);
    for(long i = 0; i < size; i++){
        vec.data[i] = NUM2DBL(rb_ary_entry(ary, i)); 
    }
    filtered = gaussian(vec, NUM2DBL(sd), 4.0);
    
    for(long i = 0; i < size; i++){
        rb_ary_store(a, i, DBL2NUM(filtered.data[i]));
    }

    Vector_destroy(vec);
    Vector_destroy(filtered);

    return a;
}

void Init_Gaussian(void){
    cGaussian = rb_define_module("Gaussian");
    rb_define_singleton_method(cGaussian, "filter1d", gaussian_filter1d, 2);
}
