CSAPP Note chap4 & chap 5

CSAPP  读书笔记系列chap4 & chap 5

#### chap4  处理器体系结构

这章说的是如何设计一个简单的处理器(Y86-64)的步骤,包括异常的处理，简单的指令跳转处理。

感觉这里说的不够详细，有点泛，个人认为更详细的可以看[计算机系统概论](https://book.douban.com/subject/2185076/)
这本，里面有设计CPU指令的硬件和dataflow。

不过，开卷有益的思想，这里主要是理解Y86的原型(X86)的流水线模型。

#####　流水线的通用原理

流水线的一个特性是提高了系统的吞吐量(throughput),类比单位时间内服务的顾客总数，即服务一个用户所需的时间。

- 吞吐量描述：GIPS(每秒千兆条指令)

处理器的实现一般使用HDL硬件描述语言Verilog，恰巧毕设的另外几个同学做的是这一块，到时候具体可以观摩一下。

另外，一般对处理器设计来说，其性能分析一般是由CPI(Cycles Per Instruction,每指令周期数)来衡量，其定义为流水线平均吞吐量的**倒数**,注意其时间单位为**时钟周期而非微微秒**

另外流水线的一般步骤为：

- 取指(fetch)： 从内存读取指令字节，地址为程序计数器PC的值

- 译码(decode): 将取值得到的指令得到相应的目标指令、寄存器以及操作数

- 执行(execute): 将操作送到ALU，可以计算一些结果和设置一些条件码

- 访存(memory): 将数据写入内存write或从内存读入数据load

- 写入(write back): 将(最多两个)接过 写到寄存器文件中

- 更新PC(PC update): 将ＰＣ设置成下一条指令的地址

总体来说，Ｃｈａｐ４　看的很快，课程计划上也没有这章，以后用到再翻吧。

#### chap5 优化程序的性能

以前一直听说**程序就等于数据结构加上算法**，似懂非懂的，读完这章可能才算理解这句话。对于一个程序，编译器会在很多方面帮助我们去优化程序的性能。例如编译器会有一些优化(gcc的优化程度： -Og -> O1(默认)->O2 ->O3)；但这些相对来说都是tricky，程序真正重要的部分还得程序员去完成。

例如下面的一个程序(n-gram 其对一个文档中出现的n单词的序列进行统计,例如当n = 1时，统计每个单词的次数；当ｎ= 2 时，统计两个单词连续的词组例如“Ｉ　ａｍ　”的次数)剖析中(剖析具体可以下面再谈)，需要的子函数调用的时间如下图

![剖析图１](http://img.blog.csdn.net/20171210150831767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

该程序运行时间为两百多秒，主要的花销在排序上Initial的插入排序上，但当插入排序换为内置的快排后，该程序的运行时间为６秒左右。如下：
![剖析图２](http://img.blog.csdn.net/20171210151222584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

更多的函数和数据结构这里不打算谈，这里说的是编译器的那些tricky吧,其实也为一些重要的编程习惯

##### 表示程序的性能方法

表示程序的性能方法，这里采用的是每元素的周期数(ＣＰＥ, Cycles Per Element)，和CPI一样，其时间单位为**时钟周期而非微微秒**。一般来说，CPE越小越好

条例一，以const 和　内敛函数代替define 这部分也可以参考effective c++;

先定义一两个函数，再通过迭代来说明优化的步骤

```
#include <ctype.h>
#include <stdio.h>

// OP 的操作不同，比较加法和乘法的时间

// 求和
#define IDENT (0)
#define OP (+)

/*
// 求积
#define OP (*)
#define IDENT (１)
*/

typedef long data_t；
    /* data structure for vectors */
    // 一个包括数组的长度和元素的结构
    typedef struct {
  size_t len;
  data_t *data;
} vec， *vec_ptr;

/* retrieve vector element　and store at val */
// 检测边界和得到数组的元素
int get_vec_element(*vec v, size_t idx, data_t *val) {
  if (idx >= v->len)
    return 0;
  *val = v->data[idx];
  return 1;
}

// 原始版本１，计算数组的元素积或总和
// Compute sum or product of vector elements
void combine1(vec_ptr v, data_t *dest) {
  long int i;
  *dest = IDENT;
  for (i = 0; i < vec_length(v); i++) {
    data_t val;
    get_vec_element(v, i, &val);
    *dest = *dest OP val;
  }
}

```
- 优化一　：　消去循环的低效率(通过代码移动 code motion)
例如将每次循环判断条件都不变的话(例如函数vec_length)移动到循环外，计算一次；这个一般编译器会完成
```
void combine2(vec_ptr v, data_t *dest) {
  long i;
  long length = vec_length(v); //优化二

  for (i = 0; i < length; i++) {
    data_t val;
    get_vec_element(v, i, &val);
    *dest = *dest OP val;
  }
}
```
- 优化二：　减少过程调用(这个会不同人不同看法)
因为过程调用会带来一些开销，例如将get_vec_element换为

```
void combine３(vec_ptr v, data_t *dest) {
  long i;
  long length = vec_length(v);
  data_t *d = get_vec_start(v);

  *dest = IDENT;

  for (i = 0; i < length; i++)
    *dest = *dest OP data[i];　　// 优化３
}
```

- 优化三：　减少不必要的内存引用：
combine3 中，*dest 每次都要对内存进行一次内存引用，其实不必要的，因为只是需要把最后结果存入内存就好
```
void combine4(vec_ptr v, data_t *dest) {
  long i;
  long length = vec_length(v);
  data_t *d = get_vec_start(v);
  data_t t = IDENT;   //中间变量，缓存

  for (i = 0; i < length; i++)
    t = t OP d[i];    
  *dest = t;　　　// 优化４
}
```
给出没优化和优化的对比
![优化１&4.png](http://img.blog.csdn.net/20171210162949194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

** 以下的优化需要理解处理器的基础**,其要点是提高向量指令的并行度
- 优化５(循环展开)

处理器是并行计算的，这个特性就至关重要
循环展开：一种程序变换，通过每次迭代的计算的元素的数量，减少循环的迭代次数．利用的就是并行的原理

这里的展开为"2 * 1"的展开，也就是每次循环索引ｉ加２
```
void unroll2a_combine(vec_ptr v, data_t *dest) {
  long length = vec_length(v);
  long limit = length - 1;　　　// 注意length不总是偶数，所以length-1为检测
  data_t *d = get_vec_start(v);
  data_t x = IDENT;
  long i;
  /* Combine 2 elements at a time */
  for (i = 0; i < limit; i += 2) {
    x = (x OP d[i])OP d[i + 1];
  }
  /* Finish any remaining elements */
  for (; i < length; i++) {
    x = x OP d[i];
  }
  *dest = x;
}

```
其性能还是有不少提升
![优化４＆２＊１](http://img.blog.csdn.net/20171210163950662?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 优化６　多个累积变量

也就是优化循环展开中的　(k * i)　ｉ

```
void unroll2a_combine(vec_ptr v, data_t *dest) {
  long length = vec_length(v);
  long limit = length - 1;
  data_t *d = get_vec_start(v);
  data_t x0 = IDENT;
  data_t x1 = IDENT;
  long i;
  /* Combine 2 elements at a time */
  for (i = 0; i < limit; i += 2) {
    // 注意这里的写法
    x0 = x0 OP d[i];
    x1 = x1 OP d[i + 1];
  }
  /* Finish any remaining elements */
  for (; i < length; i++) {
    x0 = x0 OP d[i];
  }
  *dest = x0 OP x1;
}
```

- 优化7　重新组合变换

例如
```
x = (x OP d[i])OP d[i + 1];
```
换为
```
x = x OP ( d[i] OP d[i + 1]);
```
对　ｄ[i] 和ｄ[i+1] 的类型为ｄｏｕｂｌｅ来说意义很大，因为其减少了计算中关键路径的操作数量，具体可以看书ｐ３７５
```

void combine7(vec_ptr v, data_t *dest) {
  long length = vec_length(v);
  long limit = length - 1;
  　　　 // 注意length不总是偶数，所以length-1为检测
      data_t *d = get_vec_start(v);
  data_t x = IDENT;
  long i;
  /* Combine 2 elements at a time */
  for (i = 0; i < limit; i += 2) {
    x = x OP(d[i] OP d[i + 1]);
    　　　 // 注意这里的写法
  }
  /* Finish any remaining elements */
  for (; i < length; i++) {
    x = x OP d[i];
  }
  *dest = x;
}
```

- 分支预测
对程序的运行的帮助也很大,尽量写出符合条件传送的代码

- 内存的性能

有一些程序的瓶颈不在算法,而在内存加载或存储中;
一般来说,加载load CPE为4, 存储store 的 CPE 为7(地址相同,结果放入缓冲区) 或 1(地址不同,两个操作独立运行)


##### 剖析程序(profile)
这里简单介绍gpro(GNU的剖析程序),同时参考chap1 的 Amdahl定律

用一个fibonacii函数做测试
```
#include "stdio.h"

int Fibonacci(int n) {
  if (n == 0)
    return 0;
  if (n == 1)
    return 1;
  return Fibonacci(n - 1) + Fibonacci(n - 2);
}

int main(int argc, char const *argv[]) {
  int i = 20;
  int res;
  res = Fibonacci(i);
  printf("%d\n", res);
  return 0;
}
```

- 在gcc 选项中加入 -pg,确保不调用内敛函数等
```
gcc -Og -pg fibo.c -o fibo
```
- 然后正常运行
```
./fibo
```
会产生一个gmon.out文件

- 然后调用gprof,将输出信息重定向到 runtime.txt文件中
```
gprof fibo > runTime.txt
```
查看剖析报告

```

index % time    self  children    called     name
                               21890             Fibonacci [1]
                0.00    0.00       1/1           main [7]
[1]      0.0    0.00    0.00       1+21890   Fibonacci [1]
                               21890             Fibonacci [1]

```
发现递归调用20次fibonacci就到了22000多次,实在恐怖!!!

#### 总结：

知道CPU的流水线设计,正确优化程序的性能,对个人理解底层的原理帮助很大.

程序的优化中,知道编译器的一些优化方法,虽然自己写程序的过程中很少会这么写,但看别人的源代码例如STL可能用得上

另外,代码优化的一个前提是在正确的代码上优化,切记一上来就优化,这可能是在错误的代码上优化,最终可能离正确道路越来越远:-(



同时,对代码的优化,应该是在程序最薄弱的地方进行的优化,在最耗时的地方优化,好钢用在刀刃上.

做优化的同时,应该写好对应的单元用例,在代码上线的时候引发的bug,正确的单元用例可以让别人信服不是自己part的问题,背少一点锅.
