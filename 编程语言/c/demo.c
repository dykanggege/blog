#include<stdio.h>

int test = 1;

enum boolean {NO,YES}; //只是把他们放在一起了，没有额外的命名空间，默认从0开始
enum boolean b = NO;

// enum bool {NO=1,YES}; 报错，NO重复定义了
// enum bool b2 = NO;

int main(void) {
    printf("%d\n",b);
    return 0;
}