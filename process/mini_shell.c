#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM 1024
#define OPT_NUM 64
char line_command[NUM];
char* argv[OPT_NUM];

int main() {
    while (1) {
        // 输出提示符
        printf("用户名@主机名 当前路径# ");
        fflush(stdout);

        // 获取用户输入， 输入的时候，输入\n
        // line_command - 1 是为了存储一个结束字'\0'
        char* s = fgets(line_command, sizeof(line_command) - 1, stdin);
        assert(s != NULL);
        (void*)s;
        line_command[strlen(line_command) - 1] = 0;
        printf("%s\n", line_command);

        // 字符串切割
        argv[0] = strtok(line_command, " ");
        int i = 1;
        while (argv[i++] = strtok(NULL, " "));

#ifdef DEBUG
        for (int i = 0; argv[i]; i++) {
            printf("argv[%d]: %s\n", i, argv[i])
        }
#endif
        //  执行命令
        pid_t pid = fork();
        assert(pid != -1);
        if (pid == 0) {
            execvp(argv[0], argv);
            exit(-1);
        }
        waitpid(pid, NULL, 0);
    }
}