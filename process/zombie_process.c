#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    pid_t id = fork();
    if (id < 0) 
    {
        perror("fork");
        return 1;
    }
    else if (id == 0) 
    {
        printf("I am child process, pid: %d, ppid: %d\n",getpid(),getppid());
        sleep(3);
        exit(1);
    }
    else
    {
        while (1) 
        {
            printf("I am parent process, pid: %d, ppid: %d\n",getpid(),getppid());
            sleep(1);
        }
    }
    return 0;
}
