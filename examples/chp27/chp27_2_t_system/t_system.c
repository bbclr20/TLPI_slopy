#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

#define MAX_CMD_LEN 200

int main(int argc, char *argv[]) {
    char str[MAX_CMD_LEN];
    int status;
    
    for (;;) {
        printf("Enter command: ");
        fflush(stdout);
    
        if (fgets(str, MAX_CMD_LEN, stdin) == NULL) {
            break;
        }

        status = system(str);
        printf("system() returned: status=0x%04x (%d,%d)\n", (unsigned int) status, status >> 8, status & 0xff);
        if (status == -1) {
            printf("system error \n");
            return -1;
        } else {
            if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
                printf("(Probably) could not invoke shell\n");
            } else { 
                /* Shell successfully executed command */
                printf("status: %d\n", status);
            }
        }
    }
    exit(EXIT_SUCCESS);
}