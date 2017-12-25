## lab6 mallocbNote
CSAPP  读书笔记系列chap9

这一次的实验是完善一个内存分配器malloc,为了节省空间,许多操作的是使用位操作,所以难度比较大.

实现的参数需要实现的参数有:
- int mm_init(void);
- void *mm_malloc(size_t size);
- void mm_free(void *ptr);

- static void *extend_heap(size_t words);
- static void *coalesce(void *bp);
- static void *find_fit(size_t asize);
- static void place(void *bp, size_t asize);

本来想使用显式的双链表的形式的方法实现,调试了两天还是出错.gdb使用价值在这里又不是很大,dump 出来的core文件实在不知道怎么分析,又到期末考试,时间问题,所以只能放弃这个实验.

这个笔记只是记载一些做这个实验的心得,对以后看STL源码分析的allocator希望有一点认识吧.

### 宏定义
```
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* 一些基本操作和宏定义 */
#define WSIZE 4 /* 4字节--字长,和头部,脚部长 */
#define DSIZE 8 /* 8字节--双字长 */
/* 块的最小大小,2^12 Byte= 4k ;Extend heap by this amount (bytes) */
#define CHUNKSIZE (1 << 12)

// 求最大值
#define MAX(x, y) ((x) > (y) ? (x) : (y))

// PACK将大小和已分配位结合起来并返回
#define PACK(size, alloc) ((size) | (alloc))

// GET得到地址(指针)p的值; PUT将val 存放在地址p,等效*p = val;
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)p = (val))

/* GET_SIZE 得到已分配地址的头部或脚步p的大小,也即 xxx & ~0x7
 *  GET_ALLOC 得到该区块的最低位,看是否已经分配,也即 xxx & ~0x1
 */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & ~0x1)

/* 给出块的指针bp,计算其头部和脚部的指针;
 * HDRP 为头部指针,FTRP为脚部指针
 */
#define HDRP(bp) ((char *)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(bp) - DSIZE)

/* 给出块的指针bp,计算其下一个块和前一个块 */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE((char *)(bp)-WSIZE))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE((char *)(bp)-DSIZE))
```
这里有点像LISP使用define,也算使用函数的一种方式

从上面代码中可以看,allocator的分配:
PACK是将一个填充的size:
最低三位表示是否分配

### 函数的实现
这里没有做完,但说一些流程:

- mm_init 链表的初始化:
 - 调用mem_sbrk来移动brk指针,然后得到brk的基础上保存list的信息(包括头部和脚部),
 - 然后调用extend_heap,将heap扩展成4k大小的一个chunk,
  - 所以说这里大小关系是:heap(用户堆,64位可以申请到几T) -> chunk(4k) -> block(双字对齐);

- mm_malloc(size_t size)至少申请2DSIZE堆空间:
 - 先调用find_fit,查找size适合的位置(可以是first fit或next fit 或best fit;这里尝试binary tree的使用二分法的best fit)
  - 若果没有找到,就需要extend_heap来增加chunk
 - 然后调用place,设置状态

- mm_free 设置head和foot的位,**没有涉及到块数据的操作,所以引用一个已free的指针的行为是未定义的**
 - 然后调用coalesce将空闲的block合并

- mm_realloc(void *ptr, size_t size)这里没有实现,直接就是naive原来的文件.
 - 其中size为0，相当于free，ptr为NULL，相当于malloc，memmove实现旧数据移动，包括缩小空间和扩大空间两种情况。

 一些调用需要的函数如下:

```
/* 一些static全局变量 */
static char *heap_listp = 0; // 指向第一个块
static void *root = NULL;
/* 函数声明*/

/* 扩展heap的大小*/
static void *extend_heap(size_t words);

/* 合并 */
static void *coalesce(void *bp);
/* 查找合适的block */
static void *find_fit(size_t asize);
/* 将block 选择合适放置 */
static void place(void *bp, size_t asize);
```

### 参考文献和代码:

- TA的指导网页:http://wdxtub.com/2016/04/16/thick-csapp-lab-6/
- 别人使用二叉树完成的代码:http://blog.csdn.net/u010560443/article/details/50611251
- hash实现的代码:http://blog.csdn.net/a2888409/article/details/47155501
- Github代码:
