#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    const char* msg0 = "hello printf\n";
    const char* msg1 = "hello fprintf\n";
    const char* msg2 = "hello fputs\n";
    const char* msg3 = "hello write\n";

    printf("%s", msg0);
    fprintf(stdout, "%s", msg1);
    fputs(msg2, stdout);

    write(1, msg3, strlen(msg3));

    fork();
    return 0;
}