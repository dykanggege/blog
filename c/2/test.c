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
    unsigned int * pi = &x;
    pi[i] = b;
    return x;
}

// 2.61
void test_2_61()
{
    int x = 0xFFFFFFFF;
    int y = 0x00000000;
    int z = 0x123456FF;
    int w = 0x00123456;
    int f = 0x11111111;

    // A ! 的特点，非 0 都会得到 0
    printf("%d %d %d %d %d\n",!~x,!~y,!~z,!~w,!~f);
    // B
    printf("%d %d %d %d %d\n",!x,!y,!z,!w,!f);
    // C
    printf("%d %d %d %d %d\n",!!(x << (sizeof(int)*8-1)),!!(y << (sizeof(int)*8-1)),!!(z << (sizeof(int)*8-1)),!!(w << (sizeof(int)*8-1)),!!(f << (sizeof(int)*8-1)));
    printf("%d %d %d %d\n", !(x >> ((sizeof(int) - 1) << 3)), !(y >> ((sizeof(int) - 1) << 3)), !(z >> ((sizeof(int) - 1) << 3)), !(w >> ((sizeof(int) - 1) << 3)));
    printf("done");
}

// 2.62
int int_shifts_are_arithmetic()
{
    int i = 0;
    if(i>>8)
        return 0;
    else 
        return 1;
}


// 2.64
int any_odd_one(unsigned x)
{
    int odd_one = 1;
    
    for(int i = 0; i < sizeof(unsigned int)*4; i++)
    {
        if((x & 0x00000001) != 1)
        {
            odd_one = 0;
            break;
        }
        x >> 2;
    }
    
    return odd_one;
}

// 2.67
int int_size_is_32()
{
    unsigned int i = 0xFFFF;
    i += 0x1;
    if(i)
    {
        return 0;
    } else return 1;
}

// 2.71
typedef unsigned packed_t;
int xbyte(packed_t word,int bytenum)
{
    
}


// 2.75
// A E 2  M 1.11 f 0.11


int main(int argc, char const *argv[]){
    int i = int_size_is_32();
    printf("%d",i);
    getchar();
    return 0;
}
