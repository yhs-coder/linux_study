#include "mstdio.h"

FILE_* fopen_(const char* path_name, const char* mode) {
    // 根据打开文件的方式，确认flags
    int flags = 0;
    if ((*mode) == 'r') {
        flags |= O_RDONLY;
    } else if ((*mode) == 'w') {
        flags = O_WRONLY | O_TRUNC | O_CREAT;
    } else if ((*mode) == 'a') {
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
    if (fd < 0) {
        const char* err = strerror(errno);
        write(2, err, strlen(err));
        return NULL;
    }
    // 创建并初始化FILE_
    FILE_* fp = (FILE_*)malloc(sizeof(FILE_));
    assert(fp);

    fp->flags = SYNC_LINE;  // 默认设置成行刷新
    fp->fileno = fd;
    fp->cap = SIZE;
    fp->size = 0;
    memset(fp->buffer, 0, SIZE);

    return fp;
}

void fwrite_(const void* ptr, int num, FILE_* fp) {
    // 将数据写入到缓冲区中
    memcpy(fp->buffer + fp->size, ptr, num);
    fp->size += num;

    // 根据缓冲区刷新策略刷新测试
    if (fp->flags & SYNC_NOW) {
        write(fp->fileno, fp->buffer, fp->size);
        // 标志位置0，清空缓冲区
        fp->size = 0;

    } else if (fp->flags & SYNC_FULL) {
        if (fp->size == fp->cap) {
            write(fp->fileno, fp->buffer, fp->size);
            fp->size = 0;
        }

    } else if (fp->flags & SYNC_LINE) {
        // 判断缓冲区最后一个字符是否为换行符'\n'
        if (fp->buffer[fp->size - 1] == '\n') {
            write(fp->fileno, fp->buffer, fp->size);
            fp->size = 0;
        }
    } else {
        //...
    }
}

void fclose_(FILE_* fp) {
    // 关闭前刷新缓冲区
    fflush_(fp);
    close(fp->fileno);
}

void fflush_(FILE_* fp) {
    // 将缓冲区数据刷新(写入)到系统
    if (fp->size > 0)
        write(fp->fileno, fp->buffer, fp->size);
    // 将数据强制要求OS进行外设刷新
    fsync(fp->fileno);
    fp->size = 0;
}