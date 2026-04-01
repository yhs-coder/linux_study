#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM 10
typedef void (*func_t)();

func_t handler_task[NUM];

// 样例任务
void task1() {
    printf("handler task1\n");
}
void task2() {
    printf("handler task2\n");
}
void task3() {
    printf("handler task3\n");
}

void load_task() {
    memset(handler_task, 0, sizeof(handler_task));
    handler_task[0] = task1;
    handler_task[1] = task2;
    handler_task[2] = task3;
}

// 进程等待
// 两种等待方式：阻塞式等待，非阻塞式等待
int main() {
    pid_t pid = fork();

    // child子进程执行
    if (pid == 0) {
        int cnt = 10;
        while (cnt) {
            printf("child running, pid: %d, ppid: %d, cnt: %d\n",
                   getpid(), getppid(), cnt--);
            sleep(1);

            // 子进程异常退出的情况 除零/野指针/访问越界
            // int* p = NULL;
            // *p = 1024;
        }
        exit(10);
    }

    // parent父进程执行
    // 加载样例任务
    load_task();
    int status = 0;

#if 1
    // ------ 进程非阻塞式等待方式 -----
    // 这里需要加个循环！
    // 进程是非阻塞式等待方式，需要轮询子进程的状态，检测子进程是否退出了
    while (1) {
        // 使用非阻塞式等待方式
        pid_t ret = waitpid(pid, &status, WNOHANG);
        if (ret == 0) {
            // waitpid调用成功 && 子进程还没退出
            // 注意，子进程任务还没执行完成，waitpid没有等待失败，仅仅是检测到子进程还没推出而已
            printf("wait done, but child process is running..., parent process running other things\n");
            for (int i = 0; handler_task[i]; i++) {
                // 采用回调的方式，在父进程空闲的时候执行其它事情/任务
                handler_task[i]();
            }
        } else if (ret > 0) {
            // waitpid调用成功 && 子进程退出了
            printf("wait success, exit code: %d, sig num: %d\n", (status >> 8) & 0xFF, status & 0x7F);
            // 退出轮询
            break;

        } else {
            // waitpid调用失败
            printf("waitpid call failed\n");
            break;
        }
        sleep(1);
    }
#else
    // ------ 进程阻塞式等待方式 -----
    // 1. 让OS释放子进程的僵尸状态
    // 2. 获取子进程的退出结果
    // 在等待期间，子进程没有退出的时候，父进程只能阻塞等待，CPU处于空闲
    int ret = waitpid(pid, &status, 0);
    if (ret > 0) {
        // 使用宏判断子进程是否正常退出
        if (WIFEXITED(status)) {
            // 判断子进程运行结果是否ok
            printf("exit code: %d\n", WEXITSTATUS(status));
        } else {
            // TODO
            printf("child process exit not normal!\n");
        }
        printf("wait success, exit code: %d, sig: %d\n", (status >> 8) & 0xFF, status & 0x7F);
    }
#endif

    return 0;
}