#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc > 2 || (argc > 1 && strcmp(argv[1], "--help") == 0)) {
        printf("%s [file]\n", argv[0]);
        return -1;
    }

    if (acct(argv[1]) == -1) {
        printf("Error acct\n");
        return -1;
    }

    printf("Process accounting %s\n", (argv[1] == NULL) ? "disabled" : "enabled");
    exit(EXIT_SUCCESS);
}