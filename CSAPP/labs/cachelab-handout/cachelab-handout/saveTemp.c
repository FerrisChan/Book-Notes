// NOT HIT
++misses;
// 加载ｔａｇ进来，并更新ｔｉｍｅ的值
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

int parse_traces(simu_cache *cache, char *line_buf, int s, int E, int b,
                 int flag) {
  int i;
  char opt;
  int addr;
  int selset, tag;
  sscanf(line_buf, " %c %x", &opt, &addr);
  selset = getSetBits(addr, s, b);
  tag = getTagBits(addr, s, b);
  for (i = 0; i < cache->line_num; i++) {
    if (1 == cache->sets[selset].lines[i].valid &&
        tag == cache->sets[selset].lines[i].tag) {
      /* Hit immediately */
      if ('M' == opt) {
        ++hits;
        ++hits;
      } else {
        ++hits;
      }
      updateAccessTime(cache, selset, i);
      return HIT;
    }
  }

  /* Not Hit */
  ++misses;
  for (i = 0; i < cache->line_num; i++) {
    if (0 == cache->sets[selset].lines[i].valid) {
      /* there is a empty line, No eviction */
      cache->sets[selset].lines[i].valid = 1;
      cache->sets[selset].lines[i].tag = tag;
      updateAccessTime(cache, selset, i);
      if ('M' == opt) {
        ++hits;
        return MISS_AND_HIT;
      } else {
        return MISS;
      }
    }
  }

  /* Need eviction */
  ++evictions;
  for (i = 0; i < cache->line_num; i++) {
    if (1 == cache->sets[selset].lines[i].access_time) {
      cache->sets[selset].lines[i].valid = 1;
      cache->sets[selset].lines[i].tag = tag;
      updateAccessTime(cache, selset, i);
      if ('M' == opt) {
        ++hits;
        return MISS_EVICTION_AND_HIT;
      } else {
        return MISS_AND_EVICTION;
      }
    }
  }

  return 0;
}
