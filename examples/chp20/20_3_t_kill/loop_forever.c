#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
    // loop forever
    int j;
    for (j = 0; ; j++) {
        printf("j: %d\n", j);
        sleep(3);
    }
}
