#include <assert.h>
#include <stdio.h>

#define FILE_NAME(number) "log.txt" #number

int main() {
    FILE* fp = fopen(FILE_NAME(1), "w");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    char buffer[64];
    while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
        buffer[strlen(buffer) - 1] = 0;
        puts(buffer);
    }
    int cnt = 5;
    while (cnt) {
        fprintf(fp, "%s:%d\n", "hello world", cnt--);
    }
    fclose(fp);
    return 0;
}
