#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // open a temp file
    int fd;
    char template[] = "/tmp/testXXXXXX";
    fd = mkstemp(template);
    if (fd == -1) {
        printf("Fail to open tep file: %s\n", template);
        exit(EXIT_FAILURE);
    }

    // get status and make sure that append flag is turn off
    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        printf("Fail to get the flags from fd\n");
        exit(EXIT_FAILURE);
    }
    printf("O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

    switch (fork()) {
        case -1:
            printf("Error during fork process\n");
            exit(EXIT_FAILURE);
        //child
        case 0:
            if (lseek(fd, 1000, SEEK_SET) == -1){
                printf("Error lseek in child process\n");
                exit(EXIT_FAILURE);
            }
            flags = fcntl(fd, F_GETFL);
            if (flags == -1) {
                printf("Fail to get the flags from fd in child process\n");
                exit(EXIT_FAILURE);
            }
            // turn on the append flag
            flags |= O_APPEND;
            if (fcntl(fd, F_SETFL, flags) == -1) {
                printf("Fail to set the append flag in child process\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        // parent
        default:
            if (wait(NULL) == -1) {
                printf("Error to wait in parent process\n");
                exit(EXIT_FAILURE);
            }

            /* Wait for child exit */
            printf("Child has exited\n");
            printf("File offset in parent: %lld\n", (long long) lseek(fd, 0, SEEK_CUR));
            
            flags = fcntl(fd, F_GETFL);
            if (flags == -1) {
                printf("Fail to get the flag in parent process\n");
                exit(EXIT_FAILURE);
            }
            printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");
            close(fd);
            exit(EXIT_SUCCESS);
    }
}
