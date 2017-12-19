
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

代码的[github]()
