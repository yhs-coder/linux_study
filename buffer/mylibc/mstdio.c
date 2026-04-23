#include "mstdio.h"

FILE_* fopen_(const char* path_name, const char* mode) {
    // 根据打开文件的方式，确认flags
    int flags = 0;
    if ((*mode) == "r") {
        flags |= O_RDONLY;
    } else if ((*mode) == "w") {
        flags = O_WRONLY | O_TRUNC | O_CREAT;
    } else if ((*mode) == "a") {
        flags = O_WRONLY | O_APPEND | O_CREAT;
    } else {
        // TODO,目前什么也不做
    }
    int fd = 0;
    if (flags & O_RDONLY) {
        fd = open(path_name, flags);
    } else {
        fd = open(path_name, flags, 0666);
    }

    // 创建并初始化FILE_
    FILE_* fp = (FILE_*)malloc(sizeof(FILE_));
    if (fp == NULL) {
        return NULL;
    }
    fp->flags = SYNC_LINE;
    fp->fileno = fd;
    fp->cap = SIZE;
    fp->size = 0;
    memset(fp->buffer, 0, SIZE);

    return fp;
}

void fwrite_(const void* ptr, int num, FILE_* fp) {
    // 将数据填充到缓冲区中
    memcpy(fp->buffer + fp->size, ptr, num);
    fp->size += num;
}

void fclose_(FILE_* fp) {
    close(fp->fileno);
}

void fflush_(FILE_* fp) {
    // 将缓冲区数据刷新到系统
    write(fp->fileno, fp->buffer, fp->size);
    fp->size = 0;
}