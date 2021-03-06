/*
 *  (c) 2021 Murata Mitsuharu
 *  Licensed under the MIT License.
 */

#include <stdio.h>
#include <ruby.h>
#include <ruby/intern.h>
#include <numo/narray.h>
#include <stdlib.h>
#include <pthread.h>
#include "numo-gaussian.h"
#include "gaussian.h"

extern VALUE numo_cDFloat;
VALUE cGaussian;

VALUE gaussian_filter1d_ary(VALUE self, VALUE ary, VALUE sd){
    Vector filtered[16];
    VALUE a;
    Vector v[16]; 
    long size[16];
    pthread_t th[16];
    if(TYPE(ary) != T_ARRAY) return Qfalse;
    VALUE datas = rb_ary_new();
    if(TYPE(rb_ary_entry(ary, 0)) != T_ARRAY)
        rb_ary_store(datas, 0, ary); 
    else
        datas = ary;
    long data_size = RARRAY_LEN(datas);
    if(data_size > 16) return Qfalse;    
    GaussianArgsRet ga[16]; ga;
    a = rb_ary_new();
    for(int i = 0; i < data_size; i++){
        VALUE data = rb_ary_entry(datas, i);
        size[i] = RARRAY_LEN(data);
        v[i] = Vector_zeros(size[i]);
        rb_ary_store(a, i, rb_ary_new_capa(size[i]));
        for(long j = 0; j < size[i]; j++)
            v[i].data[j] = NUM2DBL(rb_ary_entry(data, j)); 
        ga[i].src_data = v[i];
        ga[i].dst_data = filtered + i;
        ga[i].truncate = 4.0;
        if(TYPE(sd) == T_FLOAT || TYPE(sd) == T_FIXNUM){
            ga[i].sd = NUM2DBL(sd);
        }else if(TYPE(sd) == T_ARRAY){
            if(RARRAY_LEN(sd) != data_size) return Qfalse;
            ga[i].sd = NUM2DBL(rb_ary_entry(sd, i));
        }else
            return Qfalse;
        void* (*gfunc[2])() = {gaussian, narray_gaussian};
        if(pthread_create(&th[i], NULL, gfunc[0], (void*)&ga[i]))
            exit(EXIT_FAILURE);
    }
    for(int i = 0; i < data_size; i++)
        if(pthread_join(th[i], NULL))
            exit(EXIT_FAILURE);
    for(int i = 0; i < data_size; i++){   
        for(long j = 0; j < size[i]; j++){
            rb_ary_store(rb_ary_entry(a, i), j, DBL2NUM(filtered[i].data[j]));
        }
        Vector_destroy(v[i]);
        Vector_destroy(filtered[i]);
    }
    if(data_size == 1) return rb_ary_entry(a, 0);
    return a;
}

VALUE gaussian_filter1d_dfloat(VALUE self, VALUE ary, VALUE sd){    
    VALUE shape = rb_funcall(ary, rb_intern("shape"), 0);
    VALUE r = rb_funcall(ary, rb_intern("clone"), 0);
    u64 row = 1, size;
    u64 dim = NUM2INT(rb_funcall(shape, rb_intern("size"), 0));

    if(dim == 1){
        size = NUM2LONG(rb_ary_entry(shape, 0));
    }else if(dim == 2){
        row = NUM2LONG(rb_ary_entry(shape, 0));
        size = NUM2LONG(rb_ary_entry(shape, 1));
    }
    if(TYPE(sd) == T_FLOAT || TYPE(sd) == T_FIXNUM){
        VALUE tmp = rb_ary_new();
        sd = rb_funcall(sd, rb_intern("to_f"), 0);
        for(u64 i = 0; i < row; i++)
            rb_ary_store(tmp, i, sd);
        sd = tmp;
    }
    sd = rb_funcall(sd, rb_intern("to_a"), 0);

    u64 sd_size = RARRAY_LEN(sd);
    if(!(sd_size == 1 || sd_size == row)) return Qfalse;

    if(dim == 1){
        VALUE sp = rb_ary_new();
        rb_ary_store(sp, 0, INT2NUM(1));
        rb_ary_store(sp, 1, LONG2NUM(size));
        VALUE a = rb_funcall(numo_cDFloat, rb_intern("zeros"), 1, sp);
        rb_funcall(a, rb_intern("[]="), 2, Qtrue, ary);
        return rb_funcall(gaussian_filter1d_dfloat(self, a, sd), rb_intern("[]"), 1, Qtrue);
    }else if(dim == 2){
        pthread_t th[row];
        GaussianArgsRetRb ga[row];
        for(u64 i = 0; i < row; i++){
            ga[i].dst = (f64*)na_get_pointer_for_read(r);
            ga[i].src = (f64*)na_get_pointer_for_read(ary); 
            ga[i].size = size; 
            ga[i].i = i;
            ga[i].sd = NUM2DBL(rb_ary_entry(sd, i));
            ga[i].truncate = 4.0;
            if(pthread_create(th + i, NULL, (void*)_gaussian_filter1d_dfloat, (void*)(ga + i)))
                exit(EXIT_FAILURE);
        }
        for(u64 i = 0; i < row; i++)
            if(pthread_join(th[i], NULL))
                exit(EXIT_FAILURE);
        return r;
    }
    return Qfalse;
}

static VALUE gaussian_filter1d(VALUE self, VALUE ary, VALUE sd){
    VALUE ary_class = rb_funcall(ary, rb_intern("class"), 0);
    
    // rb_p(na_cast_object(ary, NA_DFLOAT));
    
    if(rb_funcall(ary_class, rb_intern("=="), 1, numo_cDFloat) == Qtrue){
        return gaussian_filter1d_dfloat(self, ary, sd);
    }else if(TYPE(ary) == T_ARRAY){
        return gaussian_filter1d_ary(self, ary, sd);
    }

    // rb_funcall(rb_stderr, rb_intern("puts"), 1, );
    return Qfalse;
}

void Init_Gaussian(void){
    cGaussian = rb_define_module("Gaussian");
    rb_define_singleton_method(cGaussian, "filter1d", gaussian_filter1d, 2);
}
