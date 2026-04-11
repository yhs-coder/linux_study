#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define FILE_NAME(number) "log.txt" #number
#define FILE_NAME "log.txt"

// 每一个宏，对应的数值，只有一个比特位是1，彼此位置不重叠
#define ONE (1 << 0)
#define TWO (1 << 1)
#define THREE (1 << 2)

// ---------- C语言文件操作 ----------
void test01() {
    // -------- 权限说明 --------
    // w:  写权限，文件不存在自动创建新文件
    // w+: 读写权限
    // r:  读权限，文件不存在出错
    // r+: 读写权限，文件不存在出错
    // a:  追加操作，文件末尾写入，若文件不存在则自动创建
    // a+: 支持读取和追加操作。读取时初始位置位于文件开头，但输出内容始终追加至文件末尾
#if 1
    // 1.以w方式打开文件，细节：会自动清空原有文件内部的数据
    FILE* fp = fopen(FILE_NAME, "w");
    // a追加内容
    // FILE* fp = fopen(FILE_NAME, "a");

    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }
    int cnt = 5;
    while (cnt) {
        fprintf(fp, "%d:%s\n", cnt--, "hello world");
    }

#else
    // 2.以r方式打开文件
    FILE* fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }
    char buffer[64];

    // fgets从特定的流中读取数据到指定元素大小的缓冲区，
    // 注意，fgets是按行读取，默认会给buffer缓冲区添加反斜杠零（'\0'）
    // sizeof(buffer) - 1，是为了给fgets留一个位置去添加'\0'
    while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
        // buffer缓冲区，存储的是从流中读取到的一行数据
        // 前提：测试文件中每行数据有换行符
        // 这里strlen(buffer) - 1，目的是将字符串最后一个字符\n替换为\0
        buffer[strlen(buffer) - 1] = 0;

        // puts打印字符串时，默认带着\n自动换行
        puts(buffer);
    }
#endif
    fclose(fp);
}

// ---------- 标志位传参测试 ----------
void show(int flags) {
    if (flags & ONE)
        printf("one\n");
    if (flags & TWO)
        printf("two\n");
    if (flags & THREE)
        printf("three\n");
}
void test02() {
    show(ONE);
    printf("-----------------------\n");
    show(TWO);
    printf("-----------------------\n");
    show(ONE | TWO);
    printf("-----------------------\n");
    show(ONE | TWO | THREE);
    printf("-----------------------\n");
}

// ---------- 文件操作相关的系统调用接口 ----------
void test03() {
    int fd = open("test.log", O_WRONLY | O_CREAT, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }
}

int main() {
    // test01();
    // test02();
    test03();
    return 0;
}
