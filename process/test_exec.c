#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// ------ 进程程序替换 -----
int main() {
    printf("process is running...\n");

    execl("/usr/bin/ls", "ls", "--color=auto", "-a", "-l", NULL);
    perror("excel");

    // 当excel替换成功后，下面printf内容就没有打印出来了
    // 因为excel执行完毕的时候，原来的代码已经被全部覆盖了，，开始执行新的程序的代码了！
    printf("process  running done...\n");
}