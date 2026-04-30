#include <stdio.h>

#include "mstdio.h"

int main() {
    FILE_* fp = fopen_("./log.txt", "w");
    if (fp == NULL) {
        return 1;
    }

    int cnt = 5;
    // 数据没有带'\n'的情况
    // const char* msg = "hello world ";

    // 数据带'\n'的情况
    const char* msg = "hello world\n";
    while (1) {
        fwrite_(msg, strlen(msg), fp);
        // 强制刷新！
        fflush_(fp);
        sleep(1);
        printf("count: %d\n", cnt);
        cnt--;
        if (cnt == 0)
            break;
    }
    fclose_(fp);
    printf("进程即将退出....\n");
    sleep(2);
    return 0;
}