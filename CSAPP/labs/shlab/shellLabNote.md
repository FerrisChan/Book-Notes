
## lab５ shellLabNote
CSAPP  读书笔记系列chap8&10

这一次的实验是完善一个简单的tiny shell;
需要完善的函数有：

- eval: Main routine that parses and interprets the command line. [70 lines]
- builtin cmd: Recognizes and interprets the built-in commands: quit, fg, bg, and jobs. [25 lines]
- do_bgfg: Implements the bg and fg built-in commands. [50 lines]
- waitfg: Waits for a foreground job to complete. [20 lines]
- sigchld handler: Catches SIGCHILD signals. 80 lines]
- sigint handler: Catches SIGINT (ctrl-c) signals. [15 lines]
- sigtstp handler: Catches SIGTSTP (ctrl-z) signals. [15 lines]

其实大多数函数书上已经有例子，照猫画虎的添加，问题也不大．

实验涉及的函数，[chap8.md](http://blog.csdn.net/ferris_chan/article/details/78824743) 也说了．

需要注意的是，因为涉及到并发的原因，gdb调试反而不如printf打印日志好用，这也算是以后的工作知道日志的重要吧．

代码的[github地址](https://github.com/FerrisChan/tools-learning/tree/master/CSAPP/labs/shlab/shlab-handout)

觉得应该顺着tshref.out文件的trace 文件来一点点做，从小功能开始慢慢迭代上去．

### 步骤

#### step1
第一步需要完成的函数是eval,定义一些参数变量，这是个big jobs．
```
void eval(char *cmdline) {
  char buf[MAXLINE]; // 缓冲
  sigset_t mask;     //信号的掩码
  pid_t pid;         // 进程ＩＤ

  struct comline_tokens {
    int bgFlag; // 是否为后台运行
    int argc;
    char *argv[MAXLINE]; /*execve()需要执行的多个argv参数,指令名和parms*/
  } tokens;

  strcpy(buf, cmdline);
  tokens.bgFlag = parseline(buf, tokens.argv);
  if (NULL == tokens.argv[0])
    return; //输入空行

  if (!builtin_cmd(tokens.argv)) {
    // 不为内置函数,执行execve

    // 防止ｒａｃｅ
    sigfillset(&mask);
    // sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, NULL); //阻塞ＣＨＬＤ信号

    if ((pid = fork()) == 0) {
      //子进程
      // 取消阻塞ＣＨＬＤ信号
      sigprocmask(SIG_UNBLOCK, &mask, NULL);
      setpgid(0, 0);
      // 执行相应的指令
      if (execve(tokens.argv[0], tokens.argv, environ) < 0) {
        // file not found
        fprintf(stderr, "%s command not found\n", tokens.argv[0]);
        return;
      }
    }
    //父进程
    // 加入job后取消阻塞ＣＨＬＤ，见８．５．６
    addjob(jobs, pid, tokens.bgFlag ? BG : FG, cmdline);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    if (tokens.bgFlag == 1) {
      // 后台运行
      printf("[%d],(%d) %s", pid2jid(pid), pid, cmdline);
    } else {
      waitfg(pid);
    }
  }
  return;
}
```
#### step2

写了几行就发现需要完善builtin_cmd函数，

```
/*
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.
 */
int builtin_cmd(char **argv) {
  if (!strcmp(argv[0], "quit"))
    //　quit退出ｓｈｅｌｌ
    exit(0);
  if (!strcmp(argv[0], "&"))
    // &指令无效
    return 1;
  if (!strcmp(argv[0], "jobs")) {
    listjobs(jobs);
    return 1;
  }
  if (!strcmp(argv[0], "bg") || !strcmp(argv[0], "fg")) {
    do_bgfg(argv);
    return 1;
  }
  return 0; /* not a builtin command */
}

```

然后发现基本的功能已经实现了，比如跑一个/bin/ls

![执行ls](http://img.blog.csdn.net/20171220094636216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

但bg 和　fg的功能没实现，这个不着急，等等step4

#### step3 信号的处理

信号的处理，这里相对比较难．我这里没有做好

##### waitfg

先说等等前台job结束的函数waitfg，这个按照书上的chap8.5.7，写一个while()循环．浪费就浪费

```
/*
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) {
  // 等前台job结束,8.5.7
  sigset_t zero;
  sigfillset(&zero);
  while (pid == fgpid(jobs))
    ;
  // 暂时开放所有信号
  // sigsuspend(&zero);
  return;
}
```

结果就被打脸了，ＣＰＵ开始都是100％的运行．．．

![CPU100%.png](http://img.blog.csdn.net/20171220095411900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVycmlzX2NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当然也可以使用sigsuspend函数，这里我没有使用

##### sigint_handler
SIGINT 信号处理器，处理ctrl-c
```
void sigint_handler(int sig) {
  // 发送信号SIGINT到所有前台程序
  pid_t pid = fgpid(jobs);
  if (pid != 0) {
    // 使用SIG_CHID 杀掉子进程
    kill(-pid, SIGINT);
  }

  return;
}
```

##### sigtstp_handler

SIGTSTP,处理ctrl-z的函数
```
void sigtstp_handler(int sig) {
  pid_t pid = fgpid(jobs);
  // int jid = pid2jid(pid);
  if (pid != 0) {
    printf("SIGTSTP got there\n");
    (*getjobpid(jobs, pid)).state = ST;
    kill(-pid, SIGTSTP);
  }
  return;
}
```

##### sigchld_handler

SIG_CHID信号处理函数,
这个没做好，在父进程和子进程中，进程的调度不符合test14的要求，实在想不明白(留个坑)

```
void sigchld_handler(int sig) {
  // SIG_CHID信号处理函数
  pid_t pid;
  int status; // 检测waitpid回收子进程的退出状态,判断是由于什么原因停止或暂停的。

  while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
    if (WIFEXITED(status))
      // 子进程ｅｘｉｔ或ｒｅｔｕｒｎ正常退出
      deletejob(jobs, pid);
    if (WIFSIGNALED(status)) {
      // 因为一个未被捕获的信号终止
      // 格式：　Job [1] (26263) terminated by signal 2
      printf("Job[%d] (%d) terminated by signal %d\n", pid2jid(pid), pid,
             WSTOPSIG(status));
      deletejob(jobs, pid);
    }
  }

  return;
}
```

test16 也没通过，具体的测试结果可以看这篇http://blog.csdn.net/The_V_/article/details/46842753

分析可以看这篇http://wdxtub.com/2016/04/16/thick-csapp-lab-5/

#### step4 do_bgfg

也就是处理fg 和　bg 的命令

- bg：将一个在后台暂停的命令，变成继续执行
- fg：将后台中的命令调至前台继续运行

都是通过kill发送SIGCONT来使进程继续

```
void do_bgfg(char **argv) {
  pid_t pid;
  struct job_t *job;
  char *id = argv[1];
  if (!id) {
    printf("%s commands requires PID or %% jobid argument\n", argv[0]);
    return;
  }
  if ('%' == id[0]) {
    // 使用jobs 中的序号
    int jid = atoi(&id[1]);
    job = getjobjid(jobs, jid);
    if (job == NULL) {
      // jobs 列表中没有
      printf("%%%d No such job \n", jid);
      return;
    }
  } else if (isdigit(id[0])) {
    pid = atoi(id);
    job = getjobpid(jobs, pid);
    if (job == NULL) {
      // jobs 列表中没有
      printf("(%d) No such process \n", pid);
      return;
    }
  } else {
    printf("%s commands requires PID or %% jobid argument\n", argv[0]);
    return;
  }
  kill(-job->pid, SIGCONT);     //　发送继续的信号
  if (!strcmp(argv[0], "bg")) { /*set job state ,do it in bg or fg*/
    job->state = BG;
    printf("[%d] (%d) %s\n", job->jid, job->pid, job->cmdline);
  } else {
    // 前台运行
    job->state = FG;
    printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
    waitfg(job->pid);
  }

  return;
}
```

实验就到这里做完了．
