#include <stdio.h>
#include <stdlib.h>

// 在进程上下文中，获取环境变量的三种方式

int main(int argc, char* argv[], char* env[]) {
    // 1. 使用命令行参数 char* env[]，传入环境变量表
    for (int i = 0; env[i]; i++) {
        printf("env[%d]: %s\n", i, env[i]);
    }

    printf("------------------------------------------------------------------------------------\n");

    // 2. 使用getenv函数，获取指定名称的环境变量（推荐！）
    printf("%s\n", getenv("PATH"));
    printf("%s\n", getenv("HOME"));

    printf("------------------------------------------------------------------------------------\n");

    // 使用系统全局的二级指针 extern char**environ, 其指向了char* env[]环境变量
    extern char** environ;
    for (int i = 0; environ[i]; i++) {
        printf("%d: %s\n", i, environ[i]);
    }
    return 0;
}