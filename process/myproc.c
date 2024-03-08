#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
   // 创建子进程 -fork函数
   // fork函数执行前： 只有一个父进程
   // fork函数执行后：父进程+子进程
   pid_t id = fork();
   if (id == 0)
   {
       // 子进程
       while (1)
       {
           printf("子进程, pid: %d, ppid是:%d, id:%d\n",getpid(),getppid(),id);
           sleep(1);
       }
   }
   else if (id > 0)
   {
       // parent父进程 
       while (1)
       {
           printf("父进程, pid: %d, ppid是:%d, id:%d\n",getpid(),getppid(),id);
           sleep(2);
       }
   }
   else 
   {
       // 错误处理
   }
   printf("我是一个进程, pid: %d, ppid是:%d, id:%d\n",getpid(),getppid(),id);

   // while (1) 
   // {
   //     printf("我是一个进程, 我的ID是: %d,  父进程ID是:%d\n",getpid(),getppid());
   //     sleep(1);
   // }
    return 0;
}
