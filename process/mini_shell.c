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
int last_code = 0;
int last_sig = 0;

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
        // printf("%s\n", line_command);

        // 字符串切割
        argv[0] = strtok(line_command, " ");
        int i = 1;

        if (argv[0] != NULL & strcmp(argv[0], "ls") == 0) {
            argv[i++] = (char*)"--color=auto";
        }

        // 如果没有子串， strtok->NULL, argv[end] = NULL
        while (argv[i++] = strtok(NULL, " "));

        // 如果是cd命令，不需要创建子进程来执行，让shell自己执行对应的命令，本质是执行系统接口
        // 像这种不需要创建子进程来执行而是让shell自己执行的命令 -- 内建/内置命令

        // 像cd xxx/xxx 这种操作，直接切换到对应的文件路径
        if (argv[0] != NULL && strcmp(argv[0], "cd") == 0) {
            if (argv[1] != NULL) {
                chdir(argv[1]);
                continue;
            }
        }

        // echo $? 或者 echo "hello xxx" 这种情况的处理
        if (argv[0] != NULL && argv[1] != NULL && strcmp(argv[0], "echo") == 0) {
            if (strcmp(argv[1], "$?") == 0) {
                printf("%d, %d\n", last_code, last_sig);
            } else {
                printf("%s\n", argv[1]);
            }
            continue;
        }

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
            exit(1);
        }
        int status = 0;
        pid_t ret = waitpid(pid, &status, 0);
        assert(ret > 0);
        last_code = (status >> 8) & 0xFF;
        last_sig = (status & 0x7F);
    }
}