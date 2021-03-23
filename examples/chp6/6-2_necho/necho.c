#include<stdio.h>

int main(int argc, char *argv[]) {
    char **ptr;

    for(ptr=argv; *ptr != NULL; ptr++) {
        printf("%s\n", *ptr);
    } 

    return 0;
}