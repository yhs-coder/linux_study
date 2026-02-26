#include <stdio.h>
#include <unistd.h>

int g_val = 10;

// 查看每个进程的虚拟地址空间
// 观察多个进程读取同一个地址的变量时，出现不同的结果 ==> 进程间的独立性，每个进程的进程地址空间不同，数据层面也不同
int main() {
    pid_t id = fork();
    if (id < 0) {
        printf("fork error\n");
        return -1;
    } else if (id == 0) {
        int cnt = 0;
        while (1) {
            printf("I'm child process,\t pid: %d, ppid: %d | g_val: %d, &g_val: %p\n",
                   getpid(), getppid(), g_val, &g_val);
            sleep(1);
            cnt++;
            if (cnt == 5) {
                g_val = 1024;
                printf("子进程修改了全局变量g_val...\n");
            }
        }
    } else {
        while (1) {
            printf("I'm parent process,\t pid: %d, ppid: %d | g_val: %d, &g_val: %p\n",
                   getpid(), getppid(), g_val, &g_val);
            sleep(2);
        }
    }
    sleep(1);
    return 0;
}