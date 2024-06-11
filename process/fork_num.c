#include <stdio.h>
#include <unistd.h>

// 测试系统普通用户能创建多少个进程

int main()
{
    int cnt = 0;
    while (1)
    {
        int ret = fork();
        if (ret < 0) 
        {
            printf("fork error! cnt: %d\n",cnt);
            break;
        }
        else if (ret == 0)
        {
            // child
            while (1) sleep(1);
        }
        // parent
        cnt++;
        printf("cnt: %d\n", cnt);
    }
    return 0;
}
