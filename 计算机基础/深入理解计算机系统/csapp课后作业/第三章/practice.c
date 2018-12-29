// 3.58
// b l w q
// multiplication 乘法
// subtraction 减法
// gcc -Og 不做高级优化，生成有格式的汇编
// gcc -S any.c 生成汇编代码
// gcc -Og -c any.c 编译并汇编，生成二进制代码
// objdump -d any.o 反汇编

long long decode2(long long x, long long y, long long z)
{
    y -= z;
    x *= y;
    int i = y;
    i << 63;
    i >> 63;

    return i ^ x;
}

// 3.60
/*
x in %rdi, n in %esi
loop:
    movl    %esi, %ecx  # ecx = n
    movl    $1,   %edx  # edx(result) = 1
    movl    $0,   %eax  # eax(mask) = 0
    jmp     .L2         # 无条件跳转L2
.L3:
    movq    %rdi, %r8   # r8 = x
    andq    %rdx, %r8   # 0 and r8(x) 并将结果写入 %rdx
    orq     %r8, %rax   # r8(x) or 0
    salq    %cl, %rdx   # 算数左移 将 %cl 左移 %rdx 位
.L2:
    testq   %rdx, %rdx  # 使用 and 操作，并回写 ZF 标志位，不送回结果
    jne     .L3         # 如果 ZF 为 0，跳转 L3
*/
/* b w l q 1 2 4 8
*/
long loop(long x, int n)
{
    long result = 0;
    long mask;
    for (mask = 1; mask != 0; mask = mask << (n & 0xFF))
    {
        result |= x & mask;
    }
    return result;
}

// 3.62
typedef enum
{
    MODE_A,
    MODE_B,
    MODE_C,
    MODE_D,
    MODE_E
} mode_t;
// p1 in %rdi p2 in %rsi action in %edx
// %rax %eax 都是返回值，而 %rax 返回寄存器 8 字节，%eax 返回寄存器 4 字节
long switch3(long *p1, long *p2, mode_t action)
{
    long result = 0;

    switch (action)
    {
    case MODE_A:
        result = *p2;
        *p2 = *p1;
        break;
    case MODE_B:
        result = *p1 + *p2;   
        *p1 = result;
        break;
    case MODE_C:
        *p1 = 59;
        result = *p2;
        break;
    case MODE_D:
        result = *p2;
        *p1 = result;
        action = 27;
        break;

    case MODE_E:
        result = 27;
        break;

    default:
        action = 12;
        break;
    }

    return result;
}

// 3.65
/*
    rdx, rax = rax, rdx
    rdx += 8 a[i][j]
    rax += 120 a[j][i]
    M = 120/8
*/

// 3.67
/*
%rsp 栈指针寄存器
*/
/*
结构体是按字节线性偏移访问，传递一个结构体，分别将结构体里数值线性访问传递
栈分配是从高字节减去一定字节数，到低字节，从低字节再向高字节访问
*/