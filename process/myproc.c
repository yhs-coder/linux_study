#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    while (1)
    {
        printf("我是一个进程, 我的ID是: %d\n",getpid());
        sleep(1);
    }
    return 0;
}
