#include<stdio.h>

typedef unsigned char * byte_pointer;

// 2.55 
void show_bytes(byte_pointer pi,size_t len)
{
    for(int i = 0; i < len; i++){
        printf("%x\n",pi[i]);
    }
}

// 2.58 判断是小端还是大端
int is_big()
{
    int i = 1;
    byte_pointer pi = (byte_pointer)&i;
    return pi[0] != 1 ? 1 : 0;
}

// 2.59 假设 x y 都是 int 类型
int join_x_y(int x, int y)
{
    byte_pointer pix = (byte_pointer)&x;
    byte_pointer piy = (byte_pointer)&y;
    int val;
    byte_pointer pi_val = (byte_pointer)&val;

    int big = is_big();

    size_t x_effect_len = 0;

    if(big){
        for(size_t i = sizeof(x) - 1; i >= 0; i--)
        {
            if(pix[i] != 0)
            {
                pi_val[i] = pix[i];
                x_effect_len++;
            } 
            else break;
        }

        for(size_t i = 0; i < sizeof(y) - x_effect_len; i++)
            pi_val[i] = piy[i];
    }
    else
    {
        for(size_t i = 0; i < sizeof(x); i++)
        {
            if(pix[i] != 0)
            {
                pi_val[i] = pix[i];
                x_effect_len++;
            }
            else break;
        }

        for(size_t i = sizeof(y) - 1; i >= 0; i--)
            pi_val[i] = piy[i];
    }

    return val;
}

int main(int argc, char const *argv[]){
    int z = join_x_y(1,-1);
    printf("%d",z);

    getchar();
    return 0;
}
