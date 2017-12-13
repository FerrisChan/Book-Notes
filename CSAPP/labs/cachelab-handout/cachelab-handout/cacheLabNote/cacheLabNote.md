CSAPP Note chap6

CSAPP  读书笔记系列chap6

### lab4 cacheLabNote

这一次实验有两个ｐａｒｔ
- PART A :实现一个缓存模拟器，并分析根据给定的 trace 文件
- PART B :利用缓存来加速矩阵运算(没做)

#### PART A
用C\C++来模拟cpu对cache的访问，然后统计hits、misses和eviction的次数
其中：
- HIT 缓存命中
- MISS 缓存未命中
- EVICTION 替换：替换最久没有读取过的行.也就是LRU

编译的时候只需要简单 make clean 和 make，然后就可以进行测试了。
测试可以
- 单元测试　
```
./csim -s 4 -E 1 -b 4 -t traces/trans.trace
```

- 总的测试，查看评分
```
./test-csim
```

##### 需要解决的难点

- 如何从命令中拿到所需的参数　：　实验书上提醒给的是库函数　getopt,详细可以man getopt

- 如何从文件中读入内容:  文件指针FILE* fp ＋　函数fets()　+ 函数fscanf()

- 如何进行 cache 的存储: 动态分配一个缓冲区的大小，由s, E, b 决定

- 进行替换处理: 加入引入时间的统计，使用ＬＲＵ

###### 使用到的数据结构

```
// cache的ｌｉｎｅ定义
typedef struct {
  int valid;       /* 每行一个的有效位　*/
  int tag;         /* 每行ｔ个的标志位　*/
  int access_time; /* 每个line 的数据块的访问时间*/
} line;

// cache 的ｓｅｔ定义
typedef struct {
  line *lines;
} set;

typedef struct {
  /* cache：　S *B*E的集合，见ｐ４２６图6-25b */
  int setNum;  /*组数 */
  int lineNum; /* 每组行数 */
  set *sets;   /* 块集合 */

} csimCache;
```
##### 主函数
```
int main(int argc, char *argv[]) {
  int s = 0, E = 0,
      b = 0; // 命令行参数，组数S = 2^s; 行数E，　缓存字节块Ｂ = 2 ^b;
  char traceName[MAX]; // 输入的trace文件名
  FILE *fp;            //　文件指针
  csimCache cache;
  char lineBuff[MAX]; // 每一行的缓冲区

  getInfo(argc, argv, &s, &E, &b, traceName);
  initCache(&cache, s, E);

  fp = fopen(traceName, "r");
  if (!fp) {
    printf("Error: Cann't open file %s!\n", traceName);
    return -1;
  }

  //分析文件中每一行的缓存情况
  // fgets 从fp 读入　一行最多MAX个字符到lineBuff中
  while (NULL != fgets(lineBuff, MAX, fp)) {
    /* I  00400563,4 会比　L 7ff000370,8前少一个空格　*/
    if (' ' == lineBuff[0]) {
      lineBuff[strlen(lineBuff) - 1] = '\0';
      parseTrace(&cache, lineBuff, s, E, b);
    }
  }
  fclose(fp);
  printSummary(hits, misses, evictions);
  // destructor(&cache, s, E);
  return 0;
}
```

##### 使用到的函数

```

// 从命令行拿到相应的参数，使用函数getopt
int getInfo(int argc, char *argv[], int *ps, int *pE, int *pb, char *traceName);

// 由ｓ和Ｅ的值初始化cache
int initCache(csimCache *cache, int s, int E);

// 分析和统计每一行输入的缓冲区的情况
int parseTrace(csimCache *cache, char *lineBuff, int s, int E, int b);

// 由地址addr 得到容器set的序号　－－　组选择　
int getSetBits(int addr, int s, int b);

// 由地址addr 得到tag位　　－－　行匹配
int getTagBits(int addr, int s, int b);

// 更新访问时间,因为使用ＬＲＵ算法，替换最近最少使用的一项
int updateAccessTime(csimCache *cache, int selset, int cl);

// 析构函数，free 内存
// 但ｌａｂ里面已经嵌入释放函数，使用的时候注释掉

int destructor(csimCache *cache, int s, int E);
```

##### 　分析函数
参考的是这一篇博客：
http://blog.csdn.net/xzz_hust/article/details/8878294

##### 最终结果为：
```
./test-csim
                       Your simulator     Reference simulator
Points (s,E,b)    Hits  Misses  Evicts    Hits  Misses  Evicts
    3 (1,1,1)       9       8       6       9       8       6  traces/yi2.trace
    3 (4,2,4)       4       5       2       4       5       2  traces/yi.trace
    3 (2,1,4)       2       3       1       2       3       1  traces/dave.trace
    3 (2,1,3)     167      71      67     167      71      67  traces/trans.trace
    3 (2,2,3)     201      37      29     201      37      29  traces/trans.trace
    3 (2,4,3)     212      26      10     212      26      10  traces/trans.trace
    3 (5,1,5)     231       7       0     231       7       0  traces/trans.trace
    6 (5,1,5)  265189   21775   21743  265189   21775   21743  traces/long.trace
   27


TEST_CSIM_RESULTS=27
```

##### 实验总结
- 注意指针的使用和传递，否则一不小心就seguament fault
- 注意结构体的分配内存顺序，由大到小

#### 参考
- http://www.cnblogs.com/immortal-worm/p/5549014.html
- http://blog.csdn.net/pessis1/article/details/52993195
- http://blog.csdn.net/xzz_hust/article/details/8878294
代码见


#### PART B
时间关系没打算做，可以参考这篇
http://wdxtub.com/2016/04/16/thick-csapp-lab-4/
