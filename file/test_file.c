#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// #define FILE_NAME(number) "log.txt" #number
#define FILE_NAME "log.txt"
#define LOG_NAME(number) "log.txt" #number

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
#if 0
    // ------- 写文件 -------
    // 自定义创建文件的掩码
    umask(0);
    // int fd = open("test.log", O_WRONLY | O_CREAT, 0666);
    // C语言打开文件时的 "w"方式 == 系统调用中的 O_WRONLY | O_CREAT | O_TRUNC
    int fd = open("test.log", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    int count = 5;
    char buffer[64];
    while (count) {
        // sprintf(buffer, "%s:%d\n", "hello world", count--);
        sprintf(buffer, "%s:%d\n", "hello C++", count--);
        // 向文件中写入string的时候，要不要+1 --> no!
        // 写入文件时，不要strlen(buffer) + 1,这样做会将C语言字符串的'\0'结束符写入文件中
        // C语言字符串的结束字符'\0'，和文件没有关系
        // write(fd, buffer, strlen(buffer) + 1);
        write(fd, buffer, strlen(buffer));
    }
#else
    // ------- 读文件 -------
    // 自定义创建文件的掩码
    umask(0);
    int fd = open("test.log", O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    char buffer[1024];
    // 这里从文件读取sizeof(buffer) - 1字节，-1是为了留一个字符给'\0'
    while (1) {
        ssize_t num = read(fd, buffer, sizeof(buffer) - 1);
        if (num > 0) {
            buffer[num] = 0;
            printf("%s", buffer);
        } else {
            break;
        }
    }
#endif
    close(fd);
}

// ---------- 批量创建文件，观察文件描述符增长 ----------
void test04() {
    int fd0 = open(LOG_NAME(1), O_WRONLY | O_CREAT | O_APPEND, 0666);
    int fd1 = open(LOG_NAME(2), O_WRONLY | O_CREAT | O_APPEND, 0666);
    int fd2 = open(LOG_NAME(3), O_WRONLY | O_CREAT | O_APPEND, 0666);
    int fd3 = open(LOG_NAME(4), O_WRONLY | O_CREAT | O_APPEND, 0666);
    int fd4 = open(LOG_NAME(5), O_WRONLY | O_CREAT | O_APPEND, 0666);

    printf("stdin->fd: %d\n", stdin->_fileno);
    printf("stdout->fd: %d\n", stdout->_fileno);
    printf("stderr->fd: %d\n", stderr->_fileno);
    printf("fd: %d\n", fd0);
    printf("fd: %d\n", fd1);
    printf("fd: %d\n", fd2);
    printf("fd: %d\n", fd3);
    printf("fd: %d\n", fd4);

    close(fd0);
    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
}

int main() {
    // test01();
    // test02();
    test03();
    // test04();
    return 0;
}
