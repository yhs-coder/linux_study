#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM 1024
#define OPT_NUM 64

// 重定向类型
#define NONE_REDIRECT 0
#define INPUT_REDIRECT 1
#define OUTPUT_REDIRECT 2
#define APPEND_REDIRECT 3

char line_command[NUM];
char* argv[OPT_NUM];
int last_code = 0;
int last_sig = 0;

int redirect_type = NONE_REDIRECT;
char* redirect_file = NULL;

#define trim_space(start)                \
    do {                                 \
        while (isspace(*start)) start++; \
    } while (0)

// 对输入的命令行检查处理，将其分割成 命令行 和 重定向文件信息
// 例如 ls -a > test.log，分成  ls -a 和 test.log两个部分
void command_check(char* command) {
    char* start = command;
    char* end = command + strlen(command);
    while (start < end) {
        // 输出/追加重定向判断
        if (*start == '>') {
            *start = '\0';
            start++;
            if (*start == '>') {
                redirect_type = APPEND_REDIRECT;
                start++;
            } else {
                redirect_type = OUTPUT_REDIRECT;
            }
            // 去除 >> 后面的空格
            trim_space(start);
            redirect_file = start;
            break;

        } else if (*start == '<') {  // 输入重定向判断
            *start = '\0';
            start++;
            // 去除 < 后面的空格
            trim_space(start);
            // 填写重定向的信息
            redirect_type = INPUT_REDIRECT;
            redirect_file = start;
            break;
        } else {
            start++;
        }
    }
}

int main() {
    while (1) {
        // 每次循环开始重置状态
        redirect_type = NONE_REDIRECT;
        redirect_file = NULL;
        errno = 0;

        // 输出提示符
        printf("用户名@主机名 当前路径# ");
        fflush(stdout);

        // 获取用户输入完成的时候，结尾会按回车输入换行\n
        // line_command - 1 是为了存储一个结束字'\0',fgets默认会在结尾填充，不需要自己处理
        char* s = fgets(line_command, sizeof(line_command) - 1, stdin);
        assert(s != NULL);
        (void*)s;
        // 处理结尾的换行符号\n
        line_command[strlen(line_command) - 1] = 0;
        // printf("%s\n", line_command);

        // 命令行检查
        command_check(line_command);
        // printf("command check: %s, file:%s\n", line_command, redirect_file);

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
        // 命令是子进程执行的，真正重定向的工作一定要是子进程来完成
        // 如何重定向，是父进程要给子进程提供信息的 -> 父子进程共享数据
        // 这里重定向会影响父进程吗？不会，进程具有独立性，有各自维护的文件描述符表，子进程并不会复制父进程在磁盘打开的文件
        if (pid == 0) {
            switch (redirect_type) {
            case NONE_REDIRECT:
                break;
            case INPUT_REDIRECT: {
                int fd = open(redirect_file, O_RDONLY);
                if (fd < 0) {
                    perror("open");
                    exit(errno);
                }
                // 输入重定向
                dup2(fd, 0);
            } break;
            case OUTPUT_REDIRECT:
            case APPEND_REDIRECT: {
                int flags = O_WRONLY | O_CREAT;
                if (redirect_type == APPEND_REDIRECT) {
                    flags |= O_APPEND;
                } else {
                    flags |= O_TRUNC;
                }
                int fd = open(redirect_file, flags, 0666);
                if (fd < 0) {
                    perror("open");
                    exit(2);
                }
                dup2(fd, 1);
            } break;
            default:
                break;
            }

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