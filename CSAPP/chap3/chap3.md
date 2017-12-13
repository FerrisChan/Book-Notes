CSAPP Note chap3

CSAPP  读书笔记系列chap3

### chap3 程序的机器级表示
这些读书笔记为个人读CSAPP所写，除了简单的书上重点外，也会加一些自己对计算机的感受。
文章顺序为书上目录的顺序，个人理解，应有偏颇，请见谅。

#### 3.1 历史观点

这讲为一篇关于Intel和AMD芯片的发展史

####　３.2　程序编码

再谈一次Ｃ语言编译过程

![编译过程](http://img.blog.csdn.net/20171205184751275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


一段代码及其经过编译生成的汇编代码

```
// #include <stdio.h>

// void hello(void) { printf("Hello, World!\n"); }

int main() {
  long a = 0;
  if (0 == a) {
    printf("it is l1\n");
  } else {
    printf("helloworld\n");
  }
  return 0;
}

```
对应的汇编代码

```
.file	"test.c"
.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
.string	"it is l1"
.text
.globl	main
.type	main, @function
main:
.LFB23:
.cfi_startproc
subq	$8, %rsp
.cfi_def_cfa_offset 16
movl	$.LC0, %edi
call	puts
movl	$0, %eax
addq	$8, %rsp
.cfi_def_cfa_offset 8
ret
.cfi_endproc
.LFE23:
.size	main, .-main
.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.5) 5.4.0 20160609"
.section	.note.GNU-stack,"",@progbits

```
C 语言代码被处理成了有统一格式的汇编代码，另外对于汇编格式有两种：

- AT&T 格式　如上面(本书默认)

- Intel 格式　gcc -Ｏg -S -masm=intel test.
例如：　push rbx  (具体看ｐ１１８)


两条指令来具体说明一下从 C 到汇编再到机器代码的变化：

```
// C 代码
*dest = t;
// 对应的汇编代码
movq    %rax, (%rbx)
// 对应的对象代码
0x40059e:   46 89 03
```
汇编代码，movq 就是把 8字节（ Quad words）移动到内存中 。t 的值保存在寄存器 %rax 中，dest 指向的地址保存在 %rbx 中，而 *dest 是取地址操作，对应于在内存中找到对应的值，也就是 M[%rbx]，在汇编代码中用小括号表示取地址，即 (%rbx)。 最后转换成 3 个字节的指令，并保存在 0x40059e 这个地址中。

#### 3.3 数据格式

说的是ATT 汇编中的一些约定
如:
```
- C声明　　　　　汇编后缀　　　　　大小(字节)      例子

- char        　ｂ　　　　　　　　１　　　　　　　movq
- short         w              2            movw
- int(双字)      l             4            movl
- long(四字)　　　q             4            movq
- char*(地址)　　q              4            movq
- float         s              4            movq
- double        l              8           movl

```

#### 3.4  访问信息

** 重点是理解间接寻址从而理解指针**
- x86寄存器
图片来源(http://wdxtub.com/2016/04/16/thin-csapp-2/)
![x86寄存器](http://img.blog.csdn.net/20171205191230052?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前六个寄存器(%rax, %rbx, %rcx, %rdx, %rsi, %rdi)称为通用寄存器，有其『特定』的用途：

- - %rax(%eax) 用于做累加
- - %rcx(%ecx) 用于计数

- - %rdx(%edx) 用于保存数据
- - %rbx(%ebx) 用于做内存查找的基础地址
- - %rsi(%esi) 用于保存源索引值
- - %rdi(%edi) 用于保存目标索引值

而 %rsp(%esp) 和 %rbp(%ebp) 则是作为栈指针和基指针来使用的。

#### 重点：
有些操作数是带括号的，括号的意思就是寻址，这也分两种情况：

- 普通模式，(R)，相当于 Mem[Reg[R]]，也就是说寄存器 R 指定内存地址，类似于 C 语言中的指针

- 移位模式，D(R)，相当于 Mem[Reg[R]+D]，寄存器 R 给出起始的内存地址，然后 D 是偏移量

对于寻址来说，比较通用的格式是 D(Rb, Ri, S) -> Mem[Reg[Rb]+S*Reg[Ri]+D]，其中：

D - 常数偏移量

Rb - 基寄存器

Ri - 索引寄存器，不能是 %rsp

S - 系数

除此之外，还有如下三种特殊情况

- (Rb, Ri) -> Mem[Reg[Rb]+Reg[Ri]]
- D(Rb, Ri) -> Mem[Reg[Rb]+Reg[Ri]+D]
- (Rb, Ri, S) -> Mem[Reg[Rb]+S*Reg[Ri]]

例如：
```
0x8(%rdx) = 0xf000 + 0x8 = 0xf008
(%rdx, %rcx) = 0xf000 + 0x100 = 0xf100
(%rdx, %rcx, 4) = 0xf000 + 4*0x100 = 0xf400
0x80(, %rdx, 2) = 2*0xf000 + 0x80 = 0x1e08
```

#### 3.5 算术和逻辑操作

说的是几条汇编指令，用到再翻书,和ＬＣ－３类似
例如：
- leaq S,D    # D <- S,加载地址的值到Ｄ

#### 3.6控制

也就是程序的跳转执行，跳转实际上就是根据条件代码的不同来进行不同的操作

##### 3.6.1　简单例子１
例子：
```
long absdiff(long x, long y)
{
    long result;
    if (x > y)
        result = x-y;
    else
        result = y-x;
    return result;
}

```

对应的汇编代码如下，这里 %rdi 中保存了参数 x，%rsi 中保存了参数 y，而 %rax 一般用来存储返回值：

```
absdiff:
    cmpq    %rsi, %rdi
    jle     .L4
    movq    %rdi, %rax
    subq    %rsi, %rax
    ret
.L4:    # x <= y
    movq    %rsi, %rax
    subq    %rdi, %rax

```

goto 版本，因为goto 在下面循环中十分重要

```

long absdiff_goto(long x, long y)
{
    long result;
    int ntest = x <= y;
    if (ntest) goto Else;
    result = x-y;
    goto Done;
Else:
    result = y-x;
Done:
    return result;
}
```
####＃　3.6.2 条件传送

对于　val = Test ? Then_Expr : Else_Expr;
重写为：val = x>y ? x-y : y-x;

转换成 goto 形式就是：
```
	ntest = !Test;
	if (ntest) goto Else;
	value = Then_Expr;
	goto Done;
Else:
	val = Else_Expr;
Done:
```

但是实际上汇编出来的代码，并不是这样的，会采用另一种方法来加速分支语句的执行

因为 现在的 CPU 都是依靠流水线工作的，如果程序一直是顺序的，那么这个过程就可以一直进行下去，效率会很高。但是一旦遇到分支，可能就要把流水线清空（因为后面载入的东西都错了），然后重新载入 所需要的数据，这就带来了很大的性能影响。为此用『分支预测』这一技术来解决（分支预测是另一个话题这里不展开），但是对于这类只需要判断一次的条件语句来说，其实有更好的方法。

处理器有一条指令支持 if(Test) Dest <- Src 的操作，也就是说可以不用跳转，利用条件代码来进行赋值，于是编译器在可能的时候会把上面的 goto 程序改成如下：
```
// 分支预测版本
result = Then_Expr;
eval = Else_Expr;
nt = !Test;
if (nt) result = eval;
return result;
```

具体的做法是：对与两个分支，都分别算出结果，然后利用上面的条件指令来进行赋值，这样就完美避免了因为分支可能带来的性能问题（需要清空流水线），像下面这样，同样 %rdi 中保存了参数 x，%rsi 中保存了参数 y，而 %rax 一般用来存储返回值：

absdiff:
    movq    %rdi, %rax  # x
    subq    %rsi, %rax  # result = x-y
    movq    %rsi, %rdx
    subq    %rdi, %rdx  # eval = y-x
    cmpq    %rsi, %rdi  # x:y
    cmovle  %rdx, %rax  # if <=, result = eval
    ret
这个方法不适用于：

因为会把两个分支的运算都提前算出来，如果这两个值都需要大量计算的话，就得不偿失了，所以需要分支中的计算尽量简单。
另外在涉及指针操作的时候，如 val = p ? *p : 0;，因为两个分支都会被计算，所以可能导致奇怪问题出现
最后一种就是如果分支中的计算是有副作用的，那么就不能这样弄 val = x > 0 ? x*= 7 : x+= 3;，这种情况下， x的值会改变两次

#### 3.6.3 循环的汇编实现

##### Do-While 语句以及对应使用 goto 语句进行跳转的版本：

```
// Do While 的 C 语言代码
long pcount_do(unsigned long x)
{
    long result = 0;
    do {
        result += x & 0x1;
        x >>= 1;
    } while (x);
    return result;
}
// Goto 版本
long pcount_goto(unsigned long x)
{
    long result = 0;
loop:
    result += x & 0x1;
    x >>= 1;
    if (x) goto loop;
    return result;
}
```

这个函数计算参数 x 中有多少位是 1，翻译成汇编如下：

```
    movl    $0, %eax    # result = 0
.L2:                    # loop:
    movq    %rdi, %rdx
    andl    $1, %edx    # t = x & 0x1
    addq    %rdx, %rax  # result += t
    shrq    %rdi        # x >>= 1
    jne     .L2         # if (x) goto loop
    rep; ret

```
其中 %rdi 中存储的是参数 x，%rax 存储的是返回值。换成更通用的形式如下：

```
// C Code
do
	Body
	while (Test);
// Goto Version
loop:
	Body
	if (Test)
		goto loop
```
而对于 While 语句的转换，会有两种方式
- jump to middle直接跳到中间版本，如：
```
// C While version
while (Test)
	Body
// Goto Version
	goto test;
loop:
	Body
test:
	if (Test)
		goto loop;
done:
```

- **guarded-do 版本**
如果在编译器中开启 -O1 优化，那么会把 While 先翻译成 Do-While，然后再转换成对应的 Goto 版本，因为 Do-While 语句执行起来更快，更符合 CPU 的运算模型。

```

// While version
while ( Test )
  Body
// Do-While version
if (! Test )
  goto done;
do
  Body
  while( Test );
done:

// Goto Version
if (! Test )
  goto done;
loop:
  Body
  if ( Test )
    goto loop;
done:
```


##### For 循环
也可以一步一步转换成 While 的形式，然后也会采取上面的两种等级，具体取决与优化等级

但不是所以的for都可以转为while的，具体是ｆｏｒ中有ｃｏｎｔｉｎｕｅ(参考习题３.29)
// For
for (Init; Test; Update)
	Body

// While Version
Init;
while (Test) {
	Body
	Update;
}

##### Switch 语句

一次判断会有多种可能的跳转路径。这里用一个具体的例子来进行讲解：

```
long switch_eg (long x, long y, long z){
	long w = 1;
	switch (x) {
		case 1:
			w = y*z;
			break;
		case 2:
			w = y/z;
			// fall through
		case 3:
			w += z;
			break;
		case 5:
		case 6:
			w -= z;
			break;
		default:
			w = 2;
	}
	return w;
}
```
这个例子中包含了大部分比较特殊的情况：

共享的条件：5 和 6
fall through：2 也会执行 3 的部分（这个要小心，一般来说不这么搞，如果确定要用，务必写上注释）
缺失的条件：4
具体怎么办呢？简单来说，使用跳转表（表的解决方式在很多地方都有用：**虚函数，继承甚至动态规划**），可能会类似如下汇编代码，这里 %rdi 是参数 x，%rsi 是参数 y，%rdx 是参数 z, %rax 是返回值
```
switch_eg:
    movq    %rdx, %rcx
    cmpq    $6, %rdi    # x:6
    ja      .L8
    jmp     *.L4(, %rdi, 8)
```
一个跳转表为
```
.section    .rodata
    .align 8
.L4:
    .quad   .L8 # x = 0
    .quad   .L3 # x = 1
    .quad   .L5 # x = 2
    .quad   .L9 # x = 3
    .quad   .L8 # x = 4
    .quad   .L7 # x = 5
    .quad   .L7 # x = 6

```
通过上面的例子，以大概了解处理 switch 语句的方式：大的 switch 语句会用跳转表，具体跳转时可能会用到决策树（if-elseif-elseif-else）



#### 3.7过程调用procedure
过程调用（也就是调用函数）,类似LC-3中的ｔｒａｐ

在过程调用中主要涉及三个重要的方面：

- 传递控制Passing control ：包括如何开始执行过程代码，以及如何返回到开始的地方
- 传递数据Passing data：包括过程需要的参数以及过程的返回值
- 内存管理Managing local data：如何在过程执行的时候分配内存，以及在返回之后释放内存
以上这三点，都是凭借机器指令实现的

过程调用离不开栈结构



过程调用的参数会：　如果参数没有超过六个，那么会放在：%rdi, %rsi, %rdx, %rcx, %r8, %r9 中。如果超过了，会另外放在一个栈中。
返回值会放在 %rax 中。

而对于每个过程调用来说，都会在栈中分配一个帧 Frames。每一帧里需要包含：
gcc 可以 info local查看
-　返回信息
- 本地存储（如果需要）
- 临时空间（如果需要）
整一帧会在过程调用的时候进行空间分配，然后在返回时进行回收，在 x86-64/Linux 中，栈帧的结构是固定的，当前的要执行的栈中包括：

- Argument Build: 需要使用的参数
- 如果不能保存在寄存器中，会把一些本地变量放在这里
- 已保存的寄存器上下文
- 老的栈帧的指针（可选）
而调用者的栈帧则包括：

返回地址（因为 call 指令被压入栈的）
调用所需的参数
具体如下图所示：

之前也有谈过的[递归](http://blog.csdn.net/ferris_chan/article/details/78311612)

一个例子
```
long pcount_r(unsigned long x) {
	if (x == 0)
		return 0;
	else
		return (x & 1) + pcount_r(x >> 1);
}
// 对应的汇编代码为：

pcount_r:
    mov     $0, %eax
    testq   %rdi, %rdi
    je      .L6
    push    %rbx
    movq    %rdi, %rbx
    andl    $1, %ebx
    shrq    %rdi
    call    pcount_r
    addq    %rbx, %rax
    popq    %rbx
.L6:
    rep; ret
```
实际执行的过程中，会不停进行压栈，直到最后返回，所以递归本身就是一个隐式的栈实现，但是系统一般对于栈的深度有限制（每次一都需要保存当前栈帧的各种数据），所以一般来说会把递归转换成显式栈来进行处理以防溢出。

#### 3.8数组分配和访问
对于　Ｔ　Ａ[N]; 在内存中分配一个Ｌ*N字节的连续区域；

注意访问数组和指针的效果：

例如对于 int val[5] 来说
![数组的引用](http://img.blog.csdn.net/20171206172316023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


多维数组

对于多维的数组，基本形式是 T A[R][C]，R 是行，C 是列，如果类型 T 占 K 个字节的话，那么数组所需要的内存是 R*C*K 字节。
```

int get_a_digit(int index, int dig)
{
    return A[index][dig];
}
// 对应的汇编代码为，这里假设 C = 5

leaq    (%rdi, %rdi, 4), %rax   # 5 * index
addl    %rax, %rsi              # 5 * index + dig
movl    A(, %rsi, 4), %eax      # M[A + 4*(5*index+dig)]
```


#### 3.9结构体


struct rec
{
    int a[4];
    size_t i;       
    struct rect *next;
};
其在内存中的排列是

![结构体.png](http://img.blog.csdn.net/20171206172832617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对齐：

如果数据类型需要 K 个字节，那么地址都必须是 K 的倍数，
另外：
- 如果数据类型需要 K 个字节，那么地址都必须是 K 的倍数－windows的原则
- 2字节数据类型的地址必须为2的倍数，较大的数据类型（int,double,float）的地址必须是4的倍数 －　Linux的原则

因为内存访问通常来说是 4 或者 8 个字节位单位的，提高指令寻址的效率　例如(,%rdi,4)

#### 3.10 在机器级程序中将控制和数据结合起来

缓冲区溢出
看下熟悉的内存布局：
图片来源http://wdxtub.com/2016/04/16/thin-csapp-2/
![内存布局](http://img.blog.csdn.net/20171206185155075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


最上面是运行时栈，有 8MB 的大小限制，一般用来保存局部变量。然后是堆，动态的内存分配会在这里处理，例如 malloc(), calloc(), new() 等。然后是数据，指的是静态分配的数据，比如说全局变量，静态变量，常量字符串。最后是共享库等可执行的机器指令，这一部分是只读的。

可以见到，栈在最上面，也就是说，栈再往上就是另一个程序的内存范围了，这种时候我们就可以通过这种方式修改内存的其他部分了。

写到这里时间的关系就不写了，看书做ｌａｂ去

#### bomb lab

配置参考这篇:http://wdxtub.com/2016/04/16/thick-csapp-lab-2/

```
// phase_1
(gdb) disas
Dump of assembler code for function phase_1:
=> 0x0000000000400ee0 <+0>:	sub    $0x8,%rsp
   0x0000000000400ee4 <+4>:	mov    $0x402400,%esi
   0x0000000000400ee9 <+9>:	callq  0x401338 <strings_not_equal>
   0x0000000000400eee <+14>:	test   %eax,%eax
   0x0000000000400ef0 <+16>:	je     0x400ef7 <phase_1+23>
   0x0000000000400ef2 <+18>:	callq  0x40143a <explode_bomb>
   0x0000000000400ef7 <+23>:	add    $0x8,%rsp
   0x0000000000400efb <+27>:	retq
End of assembler dump.

(gdb) x/s $esi
0x402400:	"Border relations with Canada have never been better."

(gdb) set args ./solution.txt
(gdb) c
The program is not being run.
(gdb) r
Starting program: /media/ferris/TODO/2CSAPP/labs/bomb/bomb/bomb ./solution.txt
Welcome to my fiendish little bomb. You have 6 phases with
which to blow yourself up. Have a nice day!

Breakpoint 1, 0x0000000000400ee0 in phase_1 ()
(gdb) c
Continuing.
Phase 1 defused. How about the next one?
afda

Breakpoint 3, 0x0000000000400efc in phase_2 ()
```
答案：x/s $esi
0x402400:	"Border relations with Canada have never been better."


```
// phase_2
(gdb) disas
Dump of assembler code for function phase_2:
=> 0x0000000000400efc <+0>:	push   %rbp
   0x0000000000400efd <+1>:	push   %rbx
   0x0000000000400efe <+2>:	sub    $0x28,%rsp
   0x0000000000400f02 <+6>:	mov    %rsp,%rsi
   0x0000000000400f05 <+9>:	callq  0x40145c <read_six_numbers>
   0x0000000000400f0a <+14>:	cmpl   $0x1,(%rsp)  　#起始值为１
   0x0000000000400f0e <+18>:	je     0x400f30 <phase_2+52>
   0x0000000000400f10 <+20>:	callq  0x40143a <explode_bomb>
   0x0000000000400f15 <+25>:	jmp    0x400f30 <phase_2+52>
   0x0000000000400f17 <+27>:	mov    -0x4(%rbx),%eax
   0x0000000000400f1a <+30>:	add    %eax,%eax　　　#每次增大一倍
   0x0000000000400f1c <+32>:	cmp    %eax,(%rbx)
   0x0000000000400f1e <+34>:	je     0x400f25 <phase_2+41>
   0x0000000000400f20 <+36>:	callq  0x40143a <explode_bomb>
   0x0000000000400f25 <+41>:	add    $0x4,%rbx
   0x0000000000400f29 <+45>:	cmp    %rbp,%rbx
   0x0000000000400f2c <+48>:	jne    0x400f17 <phase_2+27>
   0x0000000000400f2e <+50>:	jmp    0x400f3c <phase_2+64>
   0x0000000000400f30 <+52>:	lea    0x4(%rsp),%rbx
   0x0000000000400f35 <+57>:	lea    0x18(%rsp),%rbp
   0x0000000000400f3a <+62>:	jmp    0x400f17 <phase_2+27>
   0x0000000000400f3c <+64>:	add    $0x28,%rsp
   0x0000000000400f40 <+68>:	pop    %rbx
   0x0000000000400f41 <+69>:	pop    %rbp
   0x0000000000400f42 <+70>:	retq
End of assembler dump.
(gdb) s
```

答案就是 1 2 4 8 16 32。

```
// phase_3
(gdb) disas
Dump of assembler code for function phase_3:
=> 0x0000000000400f43 <+0>:	sub    $0x18,%rsp
   0x0000000000400f47 <+4>:	lea    0xc(%rsp),%rcx
   0x0000000000400f4c <+9>:	lea    0x8(%rsp),%rdx
   0x0000000000400f51 <+14>:	mov    $0x4025cf,%esi
   0x0000000000400f56 <+19>:	mov    $0x0,%eax
   0x0000000000400f5b <+24>:	callq  0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000400f60 <+29>:	cmp    $0x1,%eax
   0x0000000000400f63 <+32>:	jg     0x400f6a <phase_3+39>
   0x0000000000400f65 <+34>:	callq  0x40143a <explode_bomb>
   0x0000000000400f6a <+39>:	cmpl   $0x7,0x8(%rsp)
   0x0000000000400f6f <+44>:	ja     0x400fad <phase_3+106>
   0x0000000000400f71 <+46>:	mov    0x8(%rsp),%eax
   0x0000000000400f75 <+50>:	jmpq   *0x402470(,%rax,8)　　# switch 语句
   0x0000000000400f7c <+57>:	mov    $0xcf,%eax　　　　　　　# cf 为２０７　
   0x0000000000400f81 <+62>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f83 <+64>:	mov    $0x2c3,%eax
   0x0000000000400f88 <+69>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f8a <+71>:	mov    $0x100,%eax
   0x0000000000400f8f <+76>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f91 <+78>:	mov    $0x185,%eax
   0x0000000000400f96 <+83>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f98 <+85>:	mov    $0xce,%eax
   0x0000000000400f9d <+90>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f9f <+92>:	mov    $0x2aa,%eax
   0x0000000000400fa4 <+97>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400fa6 <+99>:	mov    $0x147,%eax
   0x0000000000400fab <+104>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400fad <+106>:	callq  0x40143a <explode_bomb>
   0x0000000000400fb2 <+111>:	mov    $0x0,%eax
   0x0000000000400fb7 <+116>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400fb9 <+118>:	mov    $0x137,%eax
   0x0000000000400fbe <+123>:	cmp    0xc(%rsp),%eax # 比较第二个数相不相等
   0x0000000000400fc2 <+127>:	je     0x400fc9 <phase_3+134>
   0x0000000000400fc4 <+129>:	callq  0x40143a <explode_bomb>
   0x0000000000400fc9 <+134>:	add    $0x18,%rsp
   0x0000000000400fcd <+138>:	retq
End of assembler dump.
```
(gdb) x/s 0x4025cf
0x4025cf:	"%d %d"
答案为　0 207

```
// phase_4
Breakpoint 6, 0x000000000040100c in phase_4 ()
(gdb) disas
Dump of assembler code for function phase_4:
=> 0x000000000040100c <+0>:	sub    $0x18,%rsp
   0x0000000000401010 <+4>:	lea    0xc(%rsp),%rcx
   0x0000000000401015 <+9>:	lea    0x8(%rsp),%rdx
   0x000000000040101a <+14>:	mov    $0x4025cf,%esi　　# 0x4025cf:	"%d %d"
   0x000000000040101f <+19>:	mov    $0x0,%eax
   0x0000000000401024 <+24>:	callq  0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000401029 <+29>:	cmp    $0x2,%eax   ＃输入两个参数
   0x000000000040102c <+32>:	jne    0x401035 <phase_4+41> # 小于两个bomb
   0x000000000040102e <+34>:	cmpl   $0xe,0x8(%rsp)   # 第一个是否>= 13
   0x0000000000401033 <+39>:	jbe    0x40103a <phase_4+46>  ＃是，跳到46
   0x0000000000401035 <+41>:	callq  0x40143a <explode_bomb>
   0x000000000040103a <+46>:	mov    $0xe,%edx　
   0x000000000040103f <+51>:	mov    $0x0,%esi
   0x0000000000401044 <+56>:	mov    0x8(%rsp),%edi
   0x0000000000401048 <+60>:	callq  0x400fce <func4>　　#递归
   0x000000000040104d <+65>:	test   %eax,%eax
   0x000000000040104f <+67>:	jne    0x401058 <phase_4+76>
   0x0000000000401051 <+69>:	cmpl   $0x0,0xc(%rsp)
   0x0000000000401056 <+74>:	je     0x40105d <phase_4+81>
   0x0000000000401058 <+76>:	callq  0x40143a <explode_bomb>
   0x000000000040105d <+81>:	add    $0x18,%rsp
   0x0000000000401061 <+85>:	retq
End of assembler dump.
(gdb) x/s $0x4025cf
Value can't be converted to integer.
(gdb) x/s 0x4025cf
0x4025cf:	"%d %d"

// func4
(gdb) disassemble func4
Dump of assembler code for function func4:
   0x0000000000400fce <+0>:	sub    $0x8,%rsp
   0x0000000000400fd2 <+4>:	mov    %edx,%eax
   0x0000000000400fd4 <+6>:	sub    %esi,%eax
   0x0000000000400fd6 <+8>:	mov    %eax,%ecx
   0x0000000000400fd8 <+10>:	shr    $0x1f,%ecx
   0x0000000000400fdb <+13>:	add    %ecx,%eax
   0x0000000000400fdd <+15>:	sar    %eax
   0x0000000000400fdf <+17>:	lea    (%rax,%rsi,1),%ecx
   0x0000000000400fe2 <+20>:	cmp    %edi,%ecx
   0x0000000000400fe4 <+22>:	jle    0x400ff2 <func4+36>
   0x0000000000400fe6 <+24>:	lea    -0x1(%rcx),%edx
   0x0000000000400fe9 <+27>:	callq  0x400fce <func4>
   0x0000000000400fee <+32>:	add    %eax,%eax
   0x0000000000400ff0 <+34>:	jmp    0x401007 <func4+57>
   0x0000000000400ff2 <+36>:	mov    $0x0,%eax
   0x0000000000400ff7 <+41>:	cmp    %edi,%ecx
   0x0000000000400ff9 <+43>:	jge    0x401007 <func4+57>
   0x0000000000400ffb <+45>:	lea    0x1(%rcx),%esi
   0x0000000000400ffe <+48>:	callq  0x400fce <func4>
   0x0000000000401003 <+53>:	lea    0x1(%rax,%rax,1),%eax
   0x0000000000401007 <+57>:	add    $0x8,%rsp
   0x000000000040100b <+61>:	retq
End of assembler dump.
(gdb)
```
#########################################################################
第一次先做到这里，有事不做了BＯＭＢ　！！！！

附上答案

Border relations with Canada have never been better.
1 2 4 8 16 32
0 207
