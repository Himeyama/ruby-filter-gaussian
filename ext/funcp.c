#include <stdio.h>

void* func1(void){
    puts("Hello, world!");
    return NULL;
}

void* func2(void){
    puts("こんにちは世界");
    return NULL;
}

int main(void){
    void* (*a[2])() = {func1, func2};
    a[1]();
    return 0;
}