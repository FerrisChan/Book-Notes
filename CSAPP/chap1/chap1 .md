CSAPP Note chap1

CSAPP  读书笔记系列chap1

这些读书笔记为个人读CSAPP所写，除了简单的书上重点外，也会加一些自己对计算机的感受。
文章顺序为书上目录的顺序，个人理解，应该有偏颇，请见谅。

chap1 是一个很重要的章节，这里提及了在计算机体系中最重要的一个概念——抽象。可惜过去三年的大学教育中，这个概念老师很少提及，具体下面再提。

#### 1.1 信息就是位加上下文

在ITer眼里，和计算机打交道往往比和人打交道简单，因为计算机中只有两个概念：0 和1。0和1组成了信息，在机器眼里，0和1永远都不会搞混。0和1更具体是在电路中的三极管的开闭。

但在人类面前，面对一大串0和1可能就很出错。之前上[计算机系统概论](https://book.douban.com/subject/2185076/)的时候，老师曾经让我们把汇编LC-3语音翻译成01机器码，过程巨蛋疼。所以，我们要对0和1进行抽象，把计算机的语音抽象成人类所能懂的语音。

对于01，第一个抽象应该就是汇编了。参考孟岩老师的[回答几个问题中的四个半抽象](http://blog.csdn.net/myan/article/details/5884695)

```
软件搞了60年，我认为真正被实践证明了的抽象，一共有四个半，分别是：

1. 机器抽象，或者称语言抽象，构造一台新的计算机或程序语言，使其能理解领域特定的语言，从而最妥帖地解决问题。这是最有力的抽象，是软件开发中的“火箭科技”。

2. 过程抽象，把一件事情看成是一系列嵌套和串接执行的标准化过程的总和，就像流水线一样。这是极为有力的抽象，因此C语言无所不能。但是层次偏低，规模增大以后带来一些挑战。

3. 函数抽象，最玄妙的一种，这个我不多说，有兴趣的去看 Structure and Interpretation of Computer Programs.

4. 面向对象抽象，把一件事情看成是一组各负其责的对象彼此之间相互收发消息，根据消息相互协作完成工作的过程。这个抽象也极为有力。

4.5 僵化的面向对象抽象，把世界看成是由层次分明、庞杂万端的类型体系“实例化”而出的对象组成的，把事情看成是这些对象之间互相收发消息、协作而成的过程。
```
#### 1.2 程序被其他程序翻译成不同的格式

这里说的是C系语音翻译成机器语言的过程,我认为是比汇编更高的抽象。因为C语言已经在汇编的角度上加入了内存等冯诺依曼机器中的概念。

![编译过程](http://img.blog.csdn.net/20171204213129166?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

#### 1.3 了解编译系统如何工作是大有好处的

每个人理解不一样2333

#### 1.4 处理器读并解释存储在内存中的指令

一个经典的图灵机包括:

- 内存memory
- ALU Processing Unit 运算单元
- IO
- Control Unit CPU

这就是一个指令在计算机执行的步骤和所需硬件。

#### 1.5 高速缓存至关重要

cache，例如ssd中cache大小对性能影响巨大，同时会在proxy lab 中用到。

#### 1.6 存储设备形成层次结构

一图胜千言

![存储设备形成层次结构](http://img.blog.csdn.net/20171204214816211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

#### 1.7 操作系统管理硬件

除了语音抽象，操作系统OS也会提供一个抽象给用户，为了安全和兼容性能。具体通过三个抽象
- 对处理器、主存和IO的抽象
- - 进程   OS对一个正在运行的程序一个抽象
- - 线程   进程的执行单元，运行在进程中，共享同样的代码和全局数据。(C语言没有线程进程概念)
- 文件 对IO的抽象 理解这句话的意思就是Linux下的一切皆文件，包括stdin，stdout流键盘，目录，网络设备等等
- 虚拟内存 为每个进程都独占地使用主存的一个抽象(对主存和存储器的抽象)
![虚拟内存](http://img.blog.csdn.net/20171204215523188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

#### 1.8 系统间利用网络通信
网络也是一个IO设备，
类似上述，具体不表


#### 1.9 重要主题

- amdahl定律 [阿姆达尔定律](https://zh.wikipedia.org/wiki/%E9%98%BF%E5%A7%86%E8%BE%BE%E5%B0%94%E5%AE%9A%E5%BE%8B)

对系统的性能一个很好的解析，即提示系统性能应该从最大，最关键或最薄弱的部分进行


- 并行和并发

- - 并发：指一个同时具有多个活动的系统
- - 并行：指的是用并发来使一个系统运行更快

这会给过程性语音带来许多麻烦,SICP中也有提[3.4并发：时间是一个本质问题](http://blog.csdn.net/ferris_chan/article/details/78533998)
#### 1.9.3 计算机系统中抽象的重要性

![计算机系统中抽象的重要性](http://img.blog.csdn.net/20171204221412825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里提到了虚拟机，java应该是对这里的更好解释


chap1 完
