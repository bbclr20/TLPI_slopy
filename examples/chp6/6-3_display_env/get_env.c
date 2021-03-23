#include<stdio.h>
#include<stdlib.h>

int main() {
    char *env_val = getenv("PATH");
    printf("PATH: %s\n", env_val);
    return 0;
}