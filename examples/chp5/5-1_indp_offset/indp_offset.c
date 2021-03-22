#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

#define OFFSET 10
#define BUFFSIZE 300

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s src_file\n", argv[0]);
        exit(-1);
    }

    // open fd1 and change the offset
    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        printf("Fail to open %s as fd1\n", argv[1]);
        exit(-1);
    }
    lseek(fd1, OFFSET, SEEK_SET);

    // open fd2 and read
    char *buff = malloc(BUFFSIZE);
    int fd2 = open(argv[1], O_RDONLY);
    int numRead = read(fd2, buff, BUFFSIZE);

    // dump buff
    for (int j=0; j<numRead; j++) {
        printf("%c", buff[j]);
    }
    printf("\n");

    // close all
    close(fd1);
    close(fd2);
    free(buff);

    return 0;
}