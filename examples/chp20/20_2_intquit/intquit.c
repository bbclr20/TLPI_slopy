#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

static int count = 0;

static void sigHandler(int sig) {
    if (sig == SIGINT) {
        count++;
        printf("Caught SIGINT (%d)\n", count);
        return;
    }
  
    printf("Caught SIGQUIT - that's all folks!\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        printf("Error handling SIGINT\n");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGQUIT, sigHandler) == SIG_ERR) {
        printf("Error handling SIGQUIT\n");
        exit(EXIT_FAILURE);
    }

    // loop forever
    int j;
    for (j = 0; ; j++) {
        printf("j: %d\n", j);
        sleep(3);
    }
}
