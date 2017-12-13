CSAPP Note chap2

CSAPP  读书笔记系列chap2 信息的表示和处理

### chap2 信息的表示和处理
这些读书笔记为个人读CSAPP所写，除了简单的书上重点外，也会加一些自己对计算机的感受。
文章顺序为书上目录的顺序，个人理解，会有偏颇，请见谅。

这一章比较基础，数电和计算机系统概论也说过了，所以看的也算比较快。
章末会写下lab的环境配置和答案

#### 2.1 信息存储

可以结合chap1 笔记来看
重点是[补码](https://zh.wikipedia.org/wiki/%E4%BA%8C%E8%A3%9C%E6%95%B8)的概念

个人选用补码一个原因，而不用反码，可以消除 +0(00000000 ) 和 -0(+0 的反码11111111)的二义性，which 在计算机是不容忍的


假设字长为 w，一般定义如下的常量：
```
UMin = 0 即 000…0
UMax = 2w−12w−1 即 111…1
TMin = −2w−1−2w−1 即 100…0
TMax = 2w−1−12w−1−1 即 011…1
Minus 1(-1) 即 111…1
```

[大端小端](http://blog.csdn.net/ferris_chan/article/details/78300275)之前博客笔记写过

#### 2.2 整数表示

C语音中修饰符signed 和 unsigned，就表示有符号数和无符号数
注意signed，long long等 为修饰符，修饰后面的int(可省略)，非基本数据类型

个人认为C语音中，基本数据类型应该为：
- char  字符
- int   整数
- float  单精度浮点数，IEEE 754 标准32位表示
- double  双精度浮点数，64位表示
- char * 指针或数组
- void *  无类型指针

类型转换cast具体可参考书，一般来说是位的表示不变

类型扩展与截取

- 扩展（例如从 short int 到 int），都可以得到预期的结果
- -  无符号数：加 0
- - 有符号数：加符号位

- 截取（例如 unsigned 到 unsigned short），对于小的数字可以得到预期的结果
- - 无符号数：mod 操作
- - 有符号数：近似 mod 操作


#### 2.3 整数运算

一个重要的概念是溢出：无符号数还是有符号数，一旦用来表示数值的最高位发生了进位，超出了表达形式或者改变了符号位

#### 2.4 浮点数

一个新的概念为round(舍入)，一般为向偶数舍入

##### 偶数舍入：二进制中，舍入到最近的偶数，即如果出现在中间的情况，舍入之后最右边的值要是偶数
```
原数值       舍入结果    原因
2.8949999      2.89    不到一半，正常四舍五入
2.8950001      2.90    超过一般，正常四舍五入
2.8950000      2.90    刚好在一半时，保证最后一位是偶数，所以向上舍入
2.8850000      2.88    刚好在一半时，保证最后一位是偶数，所以向下舍入

```


#### lab1


- 资源地址为http://www.cs.cmu.edu/~213/schedule.html

- 建议配置环境为atom, 然后加上几个知乎蓝色大神推荐的插件https://www.zhihu.com/question/39938370/answer/91374563

得到的效果图如下：
可以一个窗口搞定c文档和make的使用，提高生产力
![环境配置](http://img.blog.csdn.net/20171206112722726?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- lab1的答案如下：

```
/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
// 摩根定律
int bitAnd(int x, int y) { return ~(~x | ~y); }

/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  int mask = 0xff;
  int res = (x >> (n << 3)); //一个字节×8位=左移3
  res = res & mask;
  return res;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
// 逻辑右移
// 逻辑右移，右侧补0，见p40
int logicalShift(int x, int n) {
  int mask = ~(((1 << 31) >> n) << 1);
  return mask & (x >> n);
}

/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
// 这条不懂，具体参考
// https://stackoverflow.com/questions/3815165/how-to-implement-bitcount-using-only-bitwise-operators
int bitCount(int n) {
  n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
  n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
  n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
  n = (n & 0x00ff00ff) + ((n >> 8) & 0x00ff00ff);
  n = (n & 0x0000ffff) + ((n >> 16) & 0x0000ffff);
  return n;
}

/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
// 看x是否为0即可
int bang(int x) { return (~((x | (~x + 1)) >> 31)) & 1; }
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
// 补码中最小的值为0x100000000
int tmin(void) { return 1 << 31; }
/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
// 这条一点都不懂：——（
int fitsBits(int x, int n) {
  int shiftNumber = ~n + 33; // 33?
  return !(x ^ ((x << shiftNumber) >> shiftNumber));
}
/*
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
// 参考这篇https://www.tuicool.com/articles/aEjyQnQ
int divpwr2(int x, int n) {
  // all zeros or all ones
  int signx = x >> 31;
  // int mask=(1<<n)+(-1);
  int mask = (1 << n) + (~0);
  int bias = signx & mask;
  return (x + bias) >> n;
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) { return ~x + 1; }
/*
 * isPositive - return 1 if x > 0, return 0 otherwise
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
// 看sign位是否为1和注意零
int isPositive(int x) { return !((x >> 31) | (!x)); }
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int signx = x >> 31;
  int signy = y >> 31;
  int signSame = ((x + (~y)) >> 31) & (!(signx ^ signy));
  int signDiffer = signx & (!signy);
  return signDiffer | signSame;
}
/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
// 求最高有效位右边有多少位.
// 首先把最高有效位后面的位全改成1，然后求1的个数bitCount
int ilog2(int x) { // most significant bit
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >> 16); // eg. 0010 1011 => 0011 1111
  // MSB is x & (~(x >> 1))
  // bitCount
  x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
  x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
  x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f);
  x = (x & 0x00ff00ff) + ((x >> 8) & 0x00ff00ff);
  x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);
  return x + ~0; // x - 1
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
  // NaN 和 非规范化的
  if (((uf << 1) >> 24) == 0xFF && ((uf << 9) != 0))
    return uf;
  return (1 << 31) ^ uf;
}
/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  int sign, exp, frac, bitc, tailb;

  if (x == 0)
    return 0;
  else if (x == 0x80000000)
    return 0xCF000000;

  sign = (x >> 31) & 1;
  if (sign)
    x = -x;

  // count bits to the right of MSB
  bitc = 1;
  while ((x >> bitc) != 0)
    bitc++;
  bitc--;

  exp = bitc + 127;

  x = x << (31 - bitc); // clear all those zeros to the left of MSB
  frac = (x >> 8) & 0x7FFFFF;

  // round to even (nearest)
  if (bitc > 23) {
    tailb = x & 0xFF; // byte to round

    if ((tailb > 128) || ((tailb == 128) && (frac & 1))) {
      frac += 1;
      if (frac >> 23) {
        exp += 1;
        frac = 0;
      }
    }
  }

  return (sign << 31) | (exp << 23) | frac;
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
// 指数位全为0的为非规格化数，
// 指数为1-Bias，底数为0.f，f表示小数字段。因此加倍时直接左移。

// 规格化的数加倍时指数+1。
unsigned float_twice(unsigned uf) {
  if ((uf & 0x7F800000) == 0) { // denormalized
    uf = ((uf & 0x007FFFFF) << 1) | (0x80000000 & uf);
  } else if ((uf & 0x7F800000) != 0x7F800000) { // normalized
    uf = uf + 0x00800000;
  } // NAN
  return uf;
}

```
