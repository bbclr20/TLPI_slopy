#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>

static void sigHandler(int sig) {
    printf("Ouch!\n");
}

int main(int argc, char *argv[]) {
    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        printf("Error handling signal SIGINT\n");
        exit(EXIT_FAILURE);
    }

    // loop forever
    int j;
    for (j = 0; ; j++) {
        printf("%d\n", j);
        sleep(3);
    }
}
