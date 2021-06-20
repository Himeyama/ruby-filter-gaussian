/*
 *  (c) 2021 Murata Mitsuharu
 *  Licensed under the MIT License.
 */

#include <stdio.h>
#include <ruby.h>
#include <stdlib.h>
#include <pthread.h>
#include "gaussian.h"

VALUE cGaussian;

static VALUE gaussian_filter1d(VALUE self, VALUE ary, VALUE sd){
    // rb_funcall(rb_cObject, rb_intern("p"), 1, sd);

    Vector filtered[16];
    VALUE a;
    Vector v[16]; 
    long size[16];
    pthread_t th[16];

    // 配列ではないとき終了
    if(TYPE(ary) != T_ARRAY) return Qfalse;

    // データ群の配列に変換
    VALUE datas = rb_ary_new();
    if(TYPE(rb_ary_entry(ary, 0)) != T_ARRAY){
        rb_ary_store(datas, 0, ary); 
    }else{
        datas = ary;
    }
    
    // データ数取得
    long data_size = RARRAY_LEN(datas);
    // rb_funcall(rb_cObject, rb_intern("p"), 1, LONG2NUM(data_size));
    // rb_funcall(rb_cObject, rb_intern("p"), 1, data);
    
    // データ数16以上未対応
    if(data_size > 16) return Qfalse;

    // 各データに対して
    GaussianArgsRet ga[16]; ga;
    a = rb_ary_new();
    for(int i = 0; i < data_size; i++){
        VALUE data = rb_ary_entry(datas, i);
        size[i] = RARRAY_LEN(data);
        v[i] = Vector_zeros(size[i]);
        rb_ary_store(a, i, rb_ary_new_capa(size[i]));
        // a[i] = ;
        for(long j = 0; j < size[i]; j++){
            v[i].data[j] = NUM2DBL(rb_ary_entry(data, j)); 
        }
        ga[i].src_data = v[i];
        ga[i].dst_data = filtered + i;
        ga[i].sd = NUM2DBL(sd);
        ga[i].truncate = 4.0;
        if(pthread_create(&th[i], NULL, (void*)gaussian, (void*)&ga[i]))
            exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < data_size; i++)
        if(pthread_join(th[i], NULL))
            exit(EXIT_FAILURE);
        

    for(int i = 0; i < data_size; i++){
        // 結果を格納
        for(long j = 0; j < size[i]; j++){
            rb_ary_store(rb_ary_entry(a, i), j, DBL2NUM(filtered[i].data[j]));
        }
        Vector_destroy(v[i]);
        Vector_destroy(filtered[i]);
    }

    return a;
}

void Init_Gaussian(void){
    cGaussian = rb_define_module("Gaussian");
    rb_define_singleton_method(cGaussian, "filter1d", gaussian_filter1d, 2);
}
