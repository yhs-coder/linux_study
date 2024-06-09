#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int add_to_target(int from, int to)
{
    int sum = 0;
    for (int i = from; i < to; i++)
        sum += i;
    
    exit(13);
    //_exit(14);
    //return sum;
}

int main()
{
    
    
    printf("hello wolrd");
    int ret = add_to_target(0, 100);
    printf("sum = %d\n", ret);
    while (1) sleep(1);

    // 测试exit和_exit
    //printf("hello wolrd");
    ////exit(11);   // exit终止进程后，会主动刷新缓冲区
    //_exit(12);// _exit不会主动刷新缓冲区，不会输出hello world
    //while (1) sleep(1);



    // 写代码是为了完成某件事情，当时如何得知任务跑得如何了？
    // -==> 通过进程退出码得知
 //   int num = add_to_target(1, 100);
 //   if (num == 5050)
 //       return 0;
 //   else 
 //       return 1;

    // 如何正确设定main函数的返回值？
    // ==> 如果不关心进程退出码， return 0 就ok
    // ==> 如果想要关心进程退出码，需要返回特定的数据表明特定的错误
    
    // 退出码的意义：0:success, !0:标识失败，


    // return 0的意思：在系统层面上，进程退出的时候，需要有对应的退出码
    // 标定进程执行的结果是否正确
    
   // return 0;
}

