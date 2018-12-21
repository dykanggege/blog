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

// 2.59 位的布尔运算
void join_x_y()
{
	int x = 0x89ABCDEF;
	int y = 0x76543210;
    printf("%x",(0xFF & x) + (0xFFFFFF00 & y));
}

// 2.60 定义为 unsigned 则默认是 int
unsigned replace_byte(unsigned x, int i, unsigned char b){
    int * pi = &x;
    pi[i] = b;
    return x;
}

// 2.61
void test_2_61(int x)
{
    int x = 0xFFFFFFFF;
    int y = 0x00000000;
    int z = 0x123456FF;
    int w = 0x00123456;

    printf("%d %d %d %d",!~x,!~y,!~z,!~w);

}


int main(int argc, char const *argv[]){
    getchar();
    return 0;
}
