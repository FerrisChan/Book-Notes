CSAPP Note chap6

CSAPP  读书笔记系列chap6

#### chap6  储存器层次结构

这一章说的是内存和缓存cache,配合之前的图

![存储设备形成层次结构](http://img.blog.csdn.net/20171210214616097?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

##### 6.1 存储技术

- 随机存取存储器(RAM, Random-Access Memory)

- - SRAM(Static RAM) 通常用在处理器做缓存
- - DRAM(Dynamic RAM) 也就是内存

- ROM

- disk传统机械硬盘 机械硬盘有许多片磁盘(platter)组成，每一片磁盘有两面；每一面由一圈圈的磁道(track)组成，而每个磁道会被分隔成不同的扇区(sector)。信息存在扇区上面

- SSD 固态硬盘 信息存储在block的页page上

- 总线BUS: 传输地址、数据和控制信号的一组电线

memory的发展历史如下:
![memory的发展.png](http://img.blog.csdn.net/20171211122917629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

##### 6.2 局部性Locality

其是为了解决访问速度过慢的问题,利用的是缓存cache的思想,其一般分为:

-时间局部性(Temporal Locality): 如果一个信息项正在被访问，那么在近期它很可能还会被再次访问

- 空间局部性(Spatial Locality): 在最近的将来将用到的信息很可能与现在正在使用的信息在空间地址上是临近的

- 顺序局部性(Order Locality): 在典型程序中，除转移类指令外，大部分指令是顺序进行的。顺序执行和非顺序执行的比例大致是5:1。此外，对大型数组访问也是顺序的。指令的顺序执行、数组的连续存放等是产生顺序局部性的原因。

而cache 的思想: 对于每个k,位于k层的更快更小的存储设备作为位于k+1岑的更大更慢的存储设备的缓存。
在第一个图中，每一层都可以看作是下一层的缓存。

更加具体的是:
![缓存类型表.png](http://img.blog.csdn.net/20171211124001662?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

##### 缓存未命中

- 缓存命中(CACHE HIT):若程序需要在第k+1层的某个数据对象d,程序会先在第k层的一个块查找,若d刚好在第k层,则为缓存命中.

- 缓存未命中　(CACHE MISS) : 命中的反面，其类型为：

- - 强制性失效(Cold/compulsory Miss): CPU 第一次访问相应缓存块，缓存中肯定没有对应数据，这是不可避免的

- - 冲突失效(Confilict Miss): 在直接相联或组相联的缓存中，不同的缓存块由于索引相同相互替换，引起的失效叫做冲突失效

- - 容量失效(Capacity Miss): 有限的缓存容量导致缓存放不下而被替换，被替换出去的缓存块再被访问，引起的失效叫做容量失效

####　高速缓存存储器　Cache Memory

这里介绍的是L1缓存的工作原理：
这里很多概念类似ｈａｓｈ　技术

对于一个缓存器：

![Ｌ１缓存器.png](http://img.blog.csdn.net/20171211160230505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 其大小容量为：　C=S×E×B　　字节

 其中：
- S 表示集合(set)数量
- E 表示数据行(line)的数量
- B 表示每个缓存块(block)保存的字节数目

而对于寻址(也就是如何寻找对应的ｂｌｏｃｋ)的方式：

由对应的地址总线去决定，也就是：
![查找过程.png](http://img.blog.csdn.net/20171211160826537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

利用的是中间位的不同，对应ｈａｓｈ的查找。这里不打算抄书。

摘抄网上的http://wdxtub.com/2016/04/16/thin-csapp-3/

```
先从 set index 确定那个 set，然后看 valid 位，接着利用 t bits 分别和每个 line 的 tag 进行比较，如果匹配则命中，那么返回 对应的位置的数据，如果不匹配，就需要替换.
```
说一下替换的算法

- 随机替换  最简单的，没使用到局部性原理

使用局部性原理：

- LRC  least recently used(最近最少使用)：替换最后一次访问时间最久远的一项(linux 内核使用)

- LFU Least frequency used(最不常使用)：替换过去某个时间引用最少的一项


#### 编写高速缓存友好(cach friendly)的代码

- 让最常见的情况运行最快

- 尽量减少每个循环内部的缓存不命中的数量

例如：　
对于一个二维数组求和：

```
int sumArrayRows( int a[M][N]){
  int i, j , sum = 0;
  for (i = 0; i< M; i++){
    for(j = 0;j <N; j++)
      sum += a[i][j];   // 先行后列
  }
  return sum;
}

```

会以下版本的空间局部性好，因为Ｃ语音是**以行优先的顺序**，当Ｍ和Ｎ较大，缓存较小的时候，命中率会大很多。


```
int sumArrayRows( int a[M][N]){
  int i, j , sum = 0;


  for(j = 0;j <N; j++){
      for (i = 0; i< M; i++)
        sum += a[i][j];　　// 先列后行
  }
  return sum;
}
```


#### 存储器山

- 读吞吐量(ｒｅａｄ　throughput): 从存储系统读数据的速率，单位为　ＭＢ/s
不同的缓存读吞吐量如下：

![存储器山.png](http://img.blog.csdn.net/20171211162959854?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

不同的机子跑起来不一样，也和ＣＰＵ、时钟频率有关

在自己的机子上跑完：

```
➜  mountain ./mountain
Clock frequency is approx. 1233.0 MHz
Memory mountain (MB/sec)
	s1	s2	s3	s4	s5	s6	s7	s8	s9	s10	s11	s12	s13	s14	s15
128m	6991	3848	2467	1839	1406	1140	954	837	751	692	652	615	601	583	573
64m	6725	3615	2302	1721	1349	1101	951	837	744	687	632	600	591	566	552
32m	6790	3639	2329	1690	1376	1114	953	864	804	740	645	605	610	579	556
16m	6860	3684	2354	1765	1386	1131	966	844	744	730	655	621	614	598	605
8m	6547	3438	2217	1669	1358	1180	1014	901	853	785	753	726	782	741	785
4m	7989	4506	3038	2332	2078	1830	1618	1447	1467	1242	1427	1413	1439	1415	1627
2m	9885	6806	5177	3954	3262	2833	2492	2236	2182	2108	2082	2036	2004	2037	2009
1024k	10064	7154	5551	4119	3397	2969	2615	2320	2259	2202	2160	2120	2103	2088	2080
512k	10322	7216	5580	4131	3391	2963	2622	2330	2257	2199	2169	2141	2141	2174	2214
256k	11048	8176	6700	5204	4219	3702	3225	2864	2817	2780	2865	2995	3091	3366	3665
128k	12732	10530	10261	8734	7088	5974	5203	4686	4647	4640	4624	4628	4687	4619	4610
64k	11298	10530	10332	8955	7270	6141	5397	4783	4705	4700	4708	4755	4614	5214	9551
32k	13000	12626	12609	12199	13671	13521	13549	13256	13478	12946	13909	13358	12946	12996	12643
16k	13186	12563	12287	11800	11880	11372	11449	10700	10585	10743	9978	12467	11508	10929	12467

```

一般来说：步进越小，空间局部性越好；工作集越小，时间局部性越好。
