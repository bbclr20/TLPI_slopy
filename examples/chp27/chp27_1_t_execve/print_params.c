#include<stdio.h>

int main(int argc, char *argv[], char **envp) {
    printf("=== argv ===\n");
    for(int i=0; i< argc; i++) {
        printf("%s\n", argv[i]);
    }

    printf("=== env ===\n");
    for(char**env=envp; *env!=0; env++) {
        printf("%s\n", *env);  
    }
    return 0;
}