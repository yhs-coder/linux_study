#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t id = fork();
    if (id == 0)
    {
        printf("I am child process, pid:%d, ppid:%d\n",getpid(),getppid());
        sleep(10);
    }
    else if (id > 0)
    {
        printf("I am parent process, pid:%d, ppid:%d\n",getpid(),getppid());
        sleep(3);
        exit(0);
    }
    else{
        perror("fork");
        return 1;
    }
    return 0;
}
