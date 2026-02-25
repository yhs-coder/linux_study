#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USER "USER"

int main() {
    char* who = getenv(USER);
    if (strcmp(who, "root") == 0) {
        printf("user: %s\n", who);
    } else {
        printf("Permission denied!\n");
    }
    return 0;
}
