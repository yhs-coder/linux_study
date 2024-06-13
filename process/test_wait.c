#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t id = fork();
    if (id == 0)
    {
        //子进程
        int cnt = 5;
        while (cnt)
        {
            printf("我是子进程: %d, 父进程: %d cnt: %d\n",getpid(), getppid(),cnt--);
            sleep(1); 
            
            // 模拟进程异常退出的情况
            
            //int *p = NULL;
            //*p = 10;
            int a = 10;
            a/=0;
        }
        
        exit(12); // 子进程退出
    }
    
    // 父进程
    //sleep(15);
    //pid_t ret = wait(NULL);
    
    // 获取子进程退出的消息

    int status = 0; // status是位图结构，低七位存储进程退出时发出的信号， 8-15位存储进程的退出码
    pid_t ret = waitpid(id, &status, 0);
    if (id > 0)
        printf("wait success: %d sig number: %d child exit code: %d\n",ret, (status & 0x7F), (status >> 8) & 0xFF);
    
    sleep(5);





}
