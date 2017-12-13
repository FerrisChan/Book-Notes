#include "cachelab.h"
#include "getopt.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX (100)

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

int hits = 0;      /* 命中　*/
int misses = 0;    /* 未命中　*/
int evictions = 0; /* 统计替换的次数　*/

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
int destructor(csimCache *cache, int s, int E);

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
  destructor(&cache, s, E);
  return 0;
}

int getInfo(int argc, char *argv[], int *ps, int *pE, int *pb,
            char *traceName) {
  int opt;
  // int countArg = 0;
  while (-1 != (opt = getopt(argc, argv, "s:E:b:t:"))) {
    switch (opt) {
    case 's':
      *ps = atoi(optarg);
      break;
    case 'E':
      *pE = atoi(optarg);
      break;
    case 'b':
      *pb = atoi(optarg);
      break;
    case 't':
      strcpy(traceName, optarg);
      break;
    default:
      printf(" wrong input\n");
      break;
    }
  }
  return 0;
}

int initCache(csimCache *cache, int s, int E) {
  cache->setNum = (2 << s); // S = 2 ^s;
  cache->lineNum = E;
  cache->sets = (set *)malloc(cache->setNum * sizeof(set));
  for (int i = 0; i < cache->setNum; ++i) {
    cache->sets[i].lines = (line *)malloc(cache->lineNum * sizeof(line));
    if (!cache->sets[i].lines) {
      fprintf(stderr, "Memory Error\n");
      exit(-1);
    }
    for (int j = 0; j < cache->lineNum; j++)
      cache->sets[i].lines[j].valid = 0; // 初始化为０，cold conficit
  }
  return 0;
}

int destructor(csimCache *cache, int s, int E) {
  int S = (2 << s);
  for (int i = 0; i < S; i++) {
    free(cache->sets[i].lines);
  }
  free(cache);
  return 0;
}

int parseTrace(csimCache *cache, char *lineBuff, int s, int E, int b) {

  char opt;
  int addr;
  int selset, tag;
  sscanf(lineBuff, " %c %x", &opt, &addr);
  selset = getSetBits(addr, s, b);
  tag = getTagBits(addr, s, b);
  // printf("%d  %d\n", selset, tag);

  for (int i = 0; i < cache->lineNum; i++) {
    if (1 == cache->sets[selset].lines[i].valid &&
        tag == cache->sets[selset].lines[i].tag) {
      if ('M' == opt) {
        ++hits;
        ++hits;
      } else {
        ++hits;
      }
      updateAccessTime(cache, selset, i);
      return 1;
    }
  }
  // NOT HIT
  ++misses;

  // 加载ｔａｇ，并更新ｔｉｍｅ的值
  for (int i = 0; i < cache->lineNum; i++) {
    // 缓冲区为空valid 为０
    if (0 == cache->sets[selset].lines[i].valid) {
      cache->sets[selset].lines[i].valid = 1;
      cache->sets[selset].lines[i].tag = tag;
      updateAccessTime(cache, selset, i);
      if ('M' == opt) {
        ++hits;
        return 2;
      } else {
        return 3;
      }
    }
  }
  // 需要替换
  ++evictions;
  for (int i = 0; i < cache->lineNum; i++) {
    // 当缓冲区有值的时候valid 为１
    if (1 == cache->sets[selset].lines[i].access_time) {
      cache->sets[selset].lines[i].valid = 1;
      cache->sets[selset].lines[i].tag = tag;
      updateAccessTime(cache, selset, i);
      if ('M' == opt) {
        ++hits;
        return 4;
      } else {
        return 5;
      }
    }
  }
  return 0;
}

int getSetBits(int addr, int s, int b) {
  int mask = 0x7fffffff >> (31 - s);
  addr = addr >> b;
  return (mask & addr);
};

int getTagBits(int addr, int s, int b) {
  int mask;
  mask = 0x7fffffff >> (31 - s - b);
  addr = addr >> (s + b);
  return (mask & addr);
}

int updateAccessTime(csimCache *cache, int selset, int cl) {
  for (int i = 0; i < cache->lineNum; i++) {
    if (1 == cache->sets[selset].lines[i].valid &&
        cache->sets[selset].lines[i].access_time >
            cache->sets[selset].lines[cl].access_time)
      --cache->sets[selset].lines[i].access_time;
  }
  cache->sets[selset].lines[cl].access_time = cache->lineNum;
  return 0;
}
