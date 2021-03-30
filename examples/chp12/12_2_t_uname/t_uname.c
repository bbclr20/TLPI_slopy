#include<sys/utsname.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {
    struct utsname uts;

    if (uname(&uts) == -1) {
        printf("uname error\n");
        return -1;
    }

    printf("Node name:   %s\n", uts.nodename);
    printf("System name: %s\n", uts.sysname);
    printf("Release:     %s\n", uts.release);
    printf("Version:     %s\n", uts.version);
    printf("Machine:     %s\n", uts.machine);
    
    exit(EXIT_SUCCESS);
}