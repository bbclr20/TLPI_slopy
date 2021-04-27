#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
    char *argVec[10]; /* Larger than required */ 
    char *envVec[] = { "GREET=salut", "BYE=adieu", NULL };

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s pathname\n", argv[0]);
        return -1;
    }

    argVec[0] = strrchr(argv[1], '/'); 
    if (argVec[0] != NULL)
        argVec[0]++;
    else
        argVec[0] = argv[1];

    argVec[1] = "hello world";
    argVec[2] = "goodbye";
    argVec[3] = NULL;

    /* Get basename from argv[1] */
    /* List must be NULL-terminated */
    execve(argv[1], argVec, envVec);
    printf("execve error"); /* If we get here, something went wrong */ 
    exit(-1);
}