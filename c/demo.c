#include<stdio.h>

enum demo{
    A = 1,
    B = 2,
    C,
    D
};


int main(void) {
    double sum, func(void);
    sum = func();
    printf("%f",sum);
    return 0;
}

double func(void) {
    return 6;
}