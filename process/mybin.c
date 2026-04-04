#include <stdio.h>
#include <stdlib.h>

int main() {
    // 系统自带的环境变量
    printf("PATH: %s\n", getenv("PATH"));
    printf("PWD: %s\n", getenv("PWD"));
    // 使用自定义环境变量
    printf("MYENV: %s\n", getenv("MYENV"));
    printf("hello C ...\n");
    return 0;
}