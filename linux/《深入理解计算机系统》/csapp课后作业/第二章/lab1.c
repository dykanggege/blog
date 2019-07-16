/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  将 return 换成解决问题的功能

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

当你解决浮点指针操作问题时，要求并不严格，你允许使用循环和条件，允许使用 int 和 unsigned，你能使用任意的 int 和 unsigned 常量

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.

明确禁止：
    1. 禁止使用宏
    2. 在该文件中定义任意额外的函数
    3. 调用任何函数
    4. 不造。。
    5. 使用任何 int 或 unsigned 外的类型
    6. 使用任何 float point 数据类型，操作或常量

NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

注意：
    1. 用 lab 编译器区检查你的解决方案正确性
    2. 每个函数使用的操作限制在 (! ~ & ^ | + << >>) 内，但是你允许调用你实现过的函数
    3. 用 harness 检查你代码正确性
    4. 用 BDD 检查你代码格式
    5. 每个函数前面都有说明，限制了它的可使用操作，如果与这里的操作有冲突，以这里注释的操作为标准

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */

#endif


#include<stdio.h>

/*
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
//    5 101 ~ 010 
//    6 011 ~ 100
//    4 001 ~ 110 
//  x & y == ~(~x | ~y)
    return ~((~x) | (~y));
}

/*
 * getByte - Extract byte n from word x 从 x 中提取第 n 个字节
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
    // 先将 x 右移 n*8 位，将要提取的字节放在最低字节上
    x >> (n << 3);
    // x & 255 保留最低字节，将其他字节清 0 
    return x & 255;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift  
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 *  x 逻辑右移 n 位，注意：我们在环境中说过了，右移是算数右移
 *  但我环境是逻辑右移
 */
int logicalShift(int x, int n) {
    x = x >> n;
//    0000|1111 101010101
//   &0000      111111111
//    1111      000000000
    int i = ~(0 >> n);
    return x & i;
}

/*
 * bitCount - returns count of number of 1's in word 这个数值二进制编码里有多少个 1
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
    // 假设我的电脑是逻辑右移
    int sum = 0;
    while(x)
    {
        int y = x >> 1;
        sum += y & 1;
    }
    return sum;
}

/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
    return (~((x|(~x + 1)) >> 31)) & 1;
}

/*
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    return 1 << 31;
}

/*
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
    /*
    n 能表示的数，除去符号位，剩下n-1位，对应到32位int数中：
    正数应该是前32-(n-1)位都是0，负数应该是32-(n-1)位都是1。
    */
    int signX = x >> 31;
    int y = x >> (n + (~0));
    return !(signX ^ y);
}

/*
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    int signX = x >> 31;
    int bias = (1 << n) + (~0);
    bias = signX & bias;
    return (x + bias) >> n;
}

/*
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    return (~x) + 1;
}
    // return !((x >> 31) | (!x));

/*
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
    return !((x >> 31) & 1);
}

/*
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    int signX = x >> 31;
    int signY = y >> 31;
    int signSame = !(signX ^ signY);
    int diff = x + (~y) + 1;
    int diffNegZero = (diff >> 31) | (!diff);
    return (signSame & diffNegZero) | ((!signSame) & signX);
}

/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
    int bn = (!!(x >> 16)) << 4;
    bn = bn + ((!!(x >> (bn + 8))) << 3);
    bn = bn + ((!!(x >> (bn + 4))) << 2);
    bn = bn + ((!!(x >> (bn + 2))) << 1);
    bn = bn + (!!(x >> (bn + 1)));
    return bn;
}

/*
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
    /*
     *  s111 1111 1xxx xxxx xxxx xxxx xxxx xxxx
     *  s is sign bit, when xs are all ZERO, this represents inf,
     *  and when xs are not all ZERO, it's NaN.
     */
    unsigned fracMask, expMask;
    unsigned fracPart, expPart;
    fracMask = (1 << 23) - 1;
    expMask = 0xff << 23;
    fracPart = uf & fracMask;
    expPart = uf & expMask;
    if ((expMask == expPart) && fracPart) {
        return uf;
    }

    return (1 << 31) + uf;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30https://www.linuxmint.com/start/sarah/
 *   Rating: 4
 */
unsigned float_i2f(int x) {
    unsigned signX, expPart, fracPart;
    unsigned absX;
    unsigned hp = 1 << 31;
    unsigned shiftLeft = 0;
    unsigned roundTail;
    unsigned result;
    if (0 == x) {
        return 0;
    }
    absX = x;
    signX = 0;
    if (x < 0) {
        absX = -x;
        signX = hp;
    }
    while (0 == (hp & absX)) {
        absX = absX << 1;
        shiftLeft += 1;
    }
    expPart = 127 + 31 - shiftLeft;
    roundTail = absX & 0xff;
    fracPart = (~(hp >> 8)) & (absX >> 8);
    result = signX | (expPart << 23) | fracPart;
    // 离大数更近时，进位；离小数更近时，舍位。
    if (roundTail > 0x80) {
        result += 1;
    } else if (0x80 == roundTail) {
        // 离两边同样近时，根据左边一位舍入到偶数，左边一位为1则进，为0则舍。
        if (fracPart & 1) {
            result += 1;
        }
    }
    return result;
}

/*
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
    unsigned signX, expPart, fracPart;
    unsigned helper = 1 << 31;
    unsigned fracMask = (1 << 23) - 1;
    if (0 == uf) {  // positive 0
        return 0;
    }
    if (helper == uf) { // negative 0
        return helper;
    }
    signX = uf & helper;
    expPart = (uf >> 23) & 0xff;
    if (expPart == 0xff) {
        return uf;
    }
    fracPart = uf & fracMask;
    if (0 == expPart) {  // 非规格化值
        fracPart = fracPart << 1;
        if (fracPart & (1 << 23)) {
            fracPart = fracPart & fracMask;
            expPart += 1;
        }
    } else {
        expPart += 1;
    }
    return signX | (expPart << 23) | fracPart;
}

int main(int argc, char const *argv[])
{
    int i = isPositive(-1);
    printf("%d",i);

    getchar();
    return 0;
}
