#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// ------ 进程程序替换 -----
int main() {
#if 1
    // ----- 创建子进程，执行全新的程序 -----
    printf("process is running...\n");
    pid_t pid = fork();
    assert(pid != -1);

    if (pid == 0) {
        // 此处的程序替换，不会影响到父进程 --> 进程具有独立性
        // l(list): 表示参数采用列表，即参数一个个传入exec*
        // execl("/usr/bin/ls", "ls", "--color=auto", "-a", "-l", NULL);

        // p(path): 表示自动搜索环境变量PATH，进行可执行程序的查找
        // 第一个ls是告诉操作系统要执行谁，后面的ls是表示怎么执行
        // execlp("ls", "ls", "--color=auto", "-a", "-l", NULL);

        // v(vector): 表示参数用数组，可以将所有的执行参数放入数组中，统一传递
        // 不使用可变参数的方式
        char* const argv_[] = {
            "ls",
            "-a",
            "-l",
            "--color=auto",
            NULL};

        // execv("/usr/bin/ls", argv_);
        // execvp("ls", argv_);

        // ----- 执行自己写的的程序 -----
        execl("./mybin", "mybin", NULL);
    }

#else
    // ----- 单进程中使用execl执行其它程序 -----
    printf("process is running...\n");

    execl("/usr/bin/ls", "ls", "--color=auto", "-a", "-l", NULL);
    perror("excel");

    // 当excel替换成功后，下面printf内容就没有打印出来了
    // 因为excel执行完毕的时候，原来的代码已经被全部覆盖了，，开始执行新的程序的代码了！
    printf("process  running done...\n");
#endif
}