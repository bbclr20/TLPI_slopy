#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 100

int main(int argc, char *argv[]) {
    int fd;
    char line[MAX_LINE];
    ssize_t n;
    char *pmax_file = "/proc/sys/kernel/pid_max";
    
    fd = open(pmax_file, (argc > 1) ? O_RDWR : O_RDONLY); 
    if (fd == -1) {
        printf("Error to open %s\n", pmax_file);
        return -1;
    }

    n = read(fd, line, MAX_LINE);
    if (n == -1) {
        printf("Fail to read from %s\n", pmax_file);
        return -1;
    }

    if (argc > 1) {
        printf("Old value: ");
    }
    printf("%.*s", (int) n, line);
    
    if (argc > 1) {
        if (write(fd, argv[1], strlen(argv[1])) != strlen(argv[1])) {
            printf("Fail to write to fd\n");
            exit(-1);
        }
        system("echo /proc/sys/kernel/pid_max now contains " "`cat /proc/sys/kernel/pid_max`");
    }
    exit(EXIT_SUCCESS);
}