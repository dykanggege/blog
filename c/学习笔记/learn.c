#include<stdio.h>
#include<stdlib.h>

void simple_var()
{
    int n = 10;
    printf("simple n %d\n",n);
    printf("simple n++ %d\n",++n);
}

void static_var()
{
    static int n = 10;
    printf("static n %d\n",n);
    printf("static n++ %d\n",++n);
}

void t_malloc()
{
    void * raw;
    double * ptd;
    int * pti;
    raw = malloc(10 * sizeof(double));

    if(ptd == NULL)
    {
        
    }
    
}


int main(int argc, char const *argv[])
{
    simple_var();
    static_var();

    simple_var();
    static_var();

    getchar();
    return 0;
}

/*
simple n 10
simple n++ 11
static n 10
static n++ 11
simple n 10
simple n++ 11
static n 11
static n++ 12
*/